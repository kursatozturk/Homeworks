import cv2
import numpy as np
import sqlite3
import re
import pickle
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
    def __init__(self, _type, *args):
        self._type = _type
        self.radius = -1 # for database issues
        if _type == Shape.RECTANGLE:
            self.points = np.array(args, dtype=np.int32)
            self.fill_black = lambda mask: cv2.rectangle(
                img = mask,
                pt1 = tuple(self.points[0]),
                pt2 = tuple(self.points[1]),
                color = (0,0,0), #BLACK COLOR
                thickness = cv2.FILLED,
            )
            self.make_visible = lambda mask: cv2.rectangle(
                img = mask,
                pt1 = tuple(self.points[0]),
                pt2 = tuple(self.points[1]),
                color = (1, 1, 1),
                thickness = cv2.FILLED,
            )
            self.blur = lambda mask: mask #will be implemented in phase 3

        elif _type == Shape.POLYLINE:
            self.points = np.int32(args)
            self.fill_black = lambda mask: cv2.fillPoly(
                img = mask,
                pts = [self.points],
                color = (0,0,0), #BLACK COLOR
            )
            self.make_visible = lambda mask: cv2.fillPoly(
                img = mask,
                pts = [self.points],
                color = (1, 1, 1),
            )
            self.blur = lambda mask: mask #will be implemented in phase 3

        elif _type == Shape.CIRCLE:
            self.radius = args[0]
            center = args[1]
            self.points = np.array(center, dtype=np.int32).reshape(1,2)
            self.fill_black = lambda mask: cv2.circle(
                img = mask,
                center = center,
                radius = self.radius,
                color = (0,0,0), # BLACK COLOR
                thickness = cv2.FILLED,
            )
            self.make_visible = lambda mask: cv2.circle(
                img = mask,
                center = center,
                radius = self.radius,
                color = (1, 1, 1),
                thickness = cv2.FILLED,
            )
            self.blur = lambda mask: mask #will be implemented in phase 3
        else:
            raise(Exception('Shape Not Understood.'))

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
    USERNAME = 1
    REGEXP = 2
    GROUPNAME = 3
    USERGROUPREFERENCE = None

    def __init__(self, _type, expr):
        self._type = _type
        self.expr = expr
        if _type == MatchExpr.USERNAME:
            self.is_match = lambda user: self.expr == user
        elif _type == MatchExpr.GROUPNAME:
            self.is_match = lambda user: MatchExpr.USERGROUPREFERENCE.ismember(user, self.expr)
        elif _type == MatchExpr.REGEXP:
            self.is_match = lambda user: re.match(self.expr, user)
        else:
            raise(Exception('Expression Not Understood.'))

class Action:
    """
        Abstract Class for permissions
    """
    def takeAction(shape, mask):
        pass

class ALLOW(Action):
    """
        Allow
    """
    def takeAction(shape, mask):
        shape.make_visible(mask)
        return True

class DENY(Action):
    """
        Fill with black the given shape
    """
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
    def takeAction(shape, mask):
        shape.blur(mask)   # Now it does not do anything
                            # It will be implemented later
        return False


class LabeledImage(object):

    db_cursor = None ## Assuming that db cursor will be held in this class for all objects

    def __init__(self, user = None):
        self.owner = user
        self.Image = None
        self.Rules = [] ###it consist of a triple : (match expresion, shape and action)
        self.default_action = ALLOW
        self.name = None

    def constructor(self, user = None):
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
        
    def loadImage(self, filepath, user = None):
        """
            Set image content from a filepath.
        """
        self.owner = user
        self.Image = cv2.imread(filepath, cv2.IMREAD_COLOR)

    def load(self, name):
        """
            Labeled images are stored on a persistent database with a unique name. 
            load() overwrites existing.
        """
        obj, self.name, self.owner = LabeledImage.db_cursor.execute('SELECT I.Object,I.name,I.owner FROM LabeledImage I WHERE I.name =?',(name,)).fetchone()


        resolved_obj = pickle._loads(obj)

        self.Image = resolved_obj['ImBytes'] # image is loaded
        #next, get rules with same order as it was added
        self.Rules = [None] * len(resolved_obj['Rules'])

        for rule in resolved_obj['Rules']:
            expr_type = rule['expr_type']
            expr = rule['expr']
            shape_type = rule['shape_type']
            radius = rule['radius']
            action = rule['action']
            points = rule['points'] # another dictionary list
            point_list = [None] * len(points )# points also needed to save order
            for point in points:
                x1 = point['x1']
                y1 = point['y1']
                posp = point['position']
                point_list[posp] = (x1, y1)
            match_expr = MatchExpr(expr_type, expr)
            if shape_type == Shape.CIRCLE:
                shape = Shape(shape_type,radius, *point_list)
            else:
                shape = Shape(shape_type, *point_list)
            pos = rule['position']
            self.Rules[pos] = (match_expr, shape,action)
        return self
    def save(self, name):
        """
            Save current labeled image to persistent storage.
        """
        self.name = name
        pickle_dict = {
            'ImBytes' : self.Image,
            'Rules' : [
            ]
        }
        for idx,rule in enumerate(self.Rules):
            m_expr, shape, action = rule
            rule_dict = {
                'expr_type' : m_expr._type,
                'expr' : m_expr.expr,
                'shape_type' : shape._type,
                'radius' : shape.radius,
                'points':[
                ],
                'action' : action,
                'position':idx,
            }
            for idp,point in enumerate(shape.points):
                point_dict = {
                    'x1' : point[0],
                    'y1' : point[1],
                    'position' : idp,
                }
                rule_dict['points'].append(point_dict)
            pickle_dict['Rules'].append(rule_dict)
        obj = pickle._dumps(pickle_dict, protocol=pickle.HIGHEST_PROTOCOL)

        LabeledImage.db_cursor.execute('INSERT INTO LabeledImage Values(NULL, ?, ?, ?)', (obj, name, self.owner))

    def setDefault(self, action : Action):
        """
            Set default action. Default is ALLOW.
        """
        self.default_action = action
        
    def addRule(self, matchexpr : MatchExpr, shape : Shape, action : Action, pos = -1):
        """
            add an access control rule.
            matchexpr and shape is described below.
            pos has default value -1 which means the end of the rule list.
        """
        if pos == -1:
            self.Rules.append((matchexpr, shape, action))
        else:
            self.Rules.insert(pos, (matchexpr, shape, action))

    def delRule(self, pos : int):
        """
            delete the rule in the given position.
        """
        self.Rules.remove(pos)

    def getImage(self, user):
        flag = True # checks if a rule is stroke

        mask = np.ones(self.Image.shape, np.uint8)
        for rule in self.Rules:
            m_expr = rule[0]
            shape = rule[1]
            action = rule[2]
            if m_expr.is_match(user):
                action.takeAction(shape, mask)
                flag = False
        if flag:
            x2, y2, _ = self.Image.shape
            self.default_action.takeAction(Shape(Shape.RECTANGLE, (0, 0), (x2, y2)), mask)

        ImCop = cv2.bitwise_and(self.Image, mask * 255)
        return ImCop