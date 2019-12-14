import cv2
import numpy as np
import re
import pickle
from user_group import UserGroup
from ast import literal_eval

class Shape:
    """
        A Shape Interface for image annotations
        
        #shape can be one of
                        (RECTANGLE, x1,y1, x2,y2),
                        (POLYLINE, [(x1,y1),...,(xn,yn)]),
                        (CIRCLE, x,y, r).
        #Each shape has two methods:
                fill_black, blur.
            Which are implemented for shape specific.
    """
    RECTANGLE = 1
    POLYLINE = 2
    CIRCLE = 3

    def __init__(self, shape_string=None, **kwargs):
        if shape_string is None:
            self._type = kwargs['_type']
            self.radius = kwargs['radius']
            args = kwargs['points']
        else:
            self._type, self.radius, self.points = Shape.resolve(shape_string)

        if self._type == Shape.RECTANGLE:
            self.fill_black = lambda mask: cv2.rectangle(
                img=mask,
                pt1=tuple(self.points[0]),
                pt2=tuple(self.points[1]),
                color=(0, 0, 0),  # BLACK COLOR
                thickness=cv2.FILLED,
            )
            self.make_visible = lambda mask: cv2.rectangle(
                img=mask,
                pt1=tuple(self.points[0]),
                pt2=tuple(self.points[1]),
                color=(1, 1, 1),
                thickness=cv2.FILLED,
            )
            self.blur = lambda mask: mask  # will be implemented in phase 3

        elif self._type == Shape.POLYLINE:
            self.fill_black = lambda mask: cv2.fillPoly(
                img=mask,
                pts = np.array([self.points], dtype=np.int32),
                color=(0, 0, 0),  # BLACK COLOR
            )
            self.make_visible = lambda mask: cv2.fillPoly(
                img=mask,
                pts= np.array([self.points], dtype=np.int32),
                color=(1, 1, 1),
            )
            self.blur = lambda mask: mask  # will be implemented in phase 3

        elif self._type == Shape.CIRCLE:
            self.fill_black = lambda mask: cv2.circle(
                img=mask,
                center=tuple(self.points[0]),
                radius=self.radius,
                color=(0, 0, 0),  # BLACK COLOR
                thickness=cv2.FILLED,
            )
            self.make_visible = lambda mask: cv2.circle(
                img=mask,
                center=tuple(self.points[0]),
                radius=self.radius,
                color=(1, 1, 1),
                thickness=cv2.FILLED,
            )
            self.blur = lambda mask: mask  # will be implemented in phase 3
        else:
            raise (Exception('Shape Not Understood.'))

    @staticmethod
    def resolve(shape_string):
        if 'CIRCLE' in shape_string:
            i = shape_string.find(',')
            j = shape_string.find(')')
            if shape_string[i + 1] == ' ':
                i += 1
            lit = shape_string[i+1:j]
            args = literal_eval(lit)
            radius = args[0]
            center = (args[1], args[2])
            return Shape.CIRCLE, radius, np.uint32((center, ))

        elif 'RECTANGLE' in shape_string:
            i = shape_string.find(',')
            j = shape_string.find(')')
            if shape_string[i+1] == ' ':
                i += 1
            lit = shape_string[i+1:j]
            args = literal_eval(lit)
            p1 = (args[0], args[1])
            p2 = (args[2], args[3])
            return Shape.RECTANGLE, -1, np.uint32((p1, p2))

        elif 'POLYLINE' in shape_string:
            i = shape_string.find('[')
            j = shape_string.find(']')
            lit = shape_string[i:j+1]
            args = literal_eval(lit)
            points = np.uint32(args)
            return Shape.POLYLINE, -1, points


class MatchExpr:
    """
    ###matchexpr is one of
                            u:username,
                            g:groupname, or
                            r:regexp. 
    #First version matches the exact username, 
        second version tests if accessing user is in the given group. 
        Third version checks if accessing username matches the regular expression.
    # MUST HOLD A REFERENCE TO UserGroup Class
    """

    def __init__(self, match_expr):
        self._type, self.expr = match_expr.split(':')
        if self._type == 'u':
            self.is_match = lambda user: self.expr == user
        elif self._type == 'r':
            self.is_match = lambda user: not re.match(self.expr, user) is None
        elif self._type == 'g':
            self.is_match = lambda user: UserGroup.ismember(user, self.expr)
        else:
            raise (Exception('match_expr type error'))


class Action:
    """
        Abstract Class for permissions
    """
    @staticmethod
    def takeAction(shape, mask):
        pass


class ALLOW(Action):
    """
        Allow
    """
    @staticmethod
    def takeAction(shape, mask):

        shape.make_visible(mask)
        return True


class DENY(Action):
    """
        Fill with black the given shape
    """
    @staticmethod
    def takeAction(shape, mask):

        try:
            shape.fill_black(mask)
            return True
        except Exception as e:
            print(e)
            return False


class BLUR(Action):
    """
        Blur the given shape
    """
    @staticmethod
    def takeAction(shape, mask):
        shape.blur(mask)  # Now it does not do anything
        # It will be implemented later
        return False


class LabeledImage(object):
    db_cursor = None  # Assuming that db cursor will be held in this class for all objects

    def __init__(self, user=None):
        self.owner = user
        self.Image = None
        self.Rules = []  # it consist of a triple : (match expresion, shape and action)
        self.default_action = ALLOW
        self.name = None

    def constructor(self, user=None):
        """
            Creates and empty image with empty access control.
        """
        return LabeledImage(user)

    def setImage(self, buffer):
        """
            Set image content of from a binary buffer. Image can be JPEG of PNG.
        """
        nparr = np.frombuffer(buffer, np.uint8)
        self.Image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    def loadImage(self, filepath):
        """
            Set image content from a filepath.
        """
        self.Image = cv2.imread(filepath, cv2.IMREAD_COLOR)

    def load(self, name):
        """
            Labeled images are stored on a persistent database with a unique name. 
            load() overwrites existing.
        """
        resp = LabeledImage.db_cursor.execute(
            'SELECT I.Object,I.name,I.owner FROM LabeledImage I WHERE I.name =?', (name,)).fetchone()

        if resp is None:
            raise(Exception('No Image Found!'))
        else:
            obj, self.name, self.owner = resp

        resolved_obj = pickle.loads(obj)

        self.Image = resolved_obj['ImBytes']  # image is loaded
        # next, get rules with same order as it was added
        self.Rules = [None] * len(resolved_obj['Rules'])
        for rule in resolved_obj['Rules']:
            expr_type = rule['expr_type']
            expr = rule['expr']
            shape_type = rule['shape_type']
            radius = rule['radius']
            action = rule['action']
            points = rule['points']  # another dictionary list
            points = [tuple(tp) for tp in points]
            match_str= ':'.join([expr_type, expr])
            match_expr = MatchExpr(match_str)
            shape_list = {Shape.CIRCLE:'CIRCLE', Shape.RECTANGLE: 'RECTANGLE', Shape.POLYLINE: 'POLYLINE'}
            if shape_type == Shape.CIRCLE:
                shape_string = '(CIRCLE, {}, {}, {})'.format(radius, points[0][0], points[0][1])
            elif shape_type == Shape.RECTANGLE:
                shape_string = '(RECTANGLE, {}, {}, {}, {})'.format(points[0][0], points[0][1], points[1][0], points[1][1])
            elif shape_type == Shape.POLYLINE:
                shape_string = '(POLYLINE, {})'.format(points)
            shape = Shape(shape_string)
            pos = rule['position']
            self.Rules[pos] = (match_expr, shape, action)
        return self

    def save(self, name):
        """
            Save current labeled image to persistent storage.
        """
        self.name = name

        pickle_dict = {
            'ImBytes': self.Image,
            'Rules': [
            ]
        }
        for idx, rule in enumerate(self.Rules):
            m_expr, shape, action = rule
            rule_dict = {
                'expr_type': m_expr._type,
                'expr': m_expr.expr,
                'shape_type': shape._type,
                'radius': shape.radius,
                'points': [
                ],
                'action': action,
                'position': idx,
            }
            for point in shape.points:
                rule_dict['points'].append(point)

            pickle_dict['Rules'].append(rule_dict)
        obj = pickle.dumps(pickle_dict, protocol=pickle.HIGHEST_PROTOCOL)

        res = LabeledImage.db_cursor.execute('SELECT * from LabeledImage WHERE name = ?', (self.name,))
        if res.fetchone():
            LabeledImage.db_cursor.execute('UPDATE LabeledImage set Object=? where name= ?', (obj, self.name))
        else:
            LabeledImage.db_cursor.execute('INSERT INTO LabeledImage Values(NULL, ?, ?, ?)', (obj, name, self.owner))

    def setDefault(self, action: Action):
        """
            Set default action. Default is ALLOW.
        """
        self.default_action = action

    def addRule(self, matchexpr: MatchExpr, shape: Shape, action: Action, pos=-1):
        """
            add an access control rule.
            matchexpr and shape is described below.
            pos has default value -1 which means the end of the rule list.
        """
        if pos == -1:
            self.Rules.append((matchexpr, shape, action))
        else:
            self.Rules.insert(pos, (matchexpr, shape, action))


    def delRule(self, pos: int):
        """
            delete the rule in the given position.
        """
        self.Rules.remove(pos)

    def getImage(self, user):
        rule_applied = False  # checks if a rule stroke
        mask = np.ones(self.Image.shape, np.uint8)
        for rule in self.Rules:
            m_expr = rule[0]
            shape = rule[1]
            action = rule[2]
            if m_expr.is_match(user):
                action.takeAction(shape, mask)
                rule_applied = True
        if not rule_applied:
            x2, y2, _ = self.Image.shape
            self.default_action.takeAction(Shape('(RECTANGLE, 0, 0, {}, {})'.format(x2, y2)), mask)

        im_cop = cv2.bitwise_and(self.Image, mask * 255)
        return im_cop
