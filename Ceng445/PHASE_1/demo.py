def construct_database(db_name):
    conn = sqlite3.connect(db_name)
    c = conn.cursor()

    c.execute("""CREATE TABLE Users(
                uname char(20),
                password char(20),
                PRIMARY KEY (uname)
                );""")

    c.execute("""CREATE TABLE Groups (
                gname char(20),
                PRIMARY KEY (gname)
                );""")

    c.execute("""CREATE TABLE isMember (
                uname char(20),
                gname char(20),
                PRIMARY KEY(uname, gname)
                FOREIGN KEY(uname) REFERENCES users ON DELETE CASCADE,
                FOREIGN KEY(gname) REFERENCES groups ON DELETE CASCADE
                );""")

    c.execute("""CREATE TABLE LabeledImage(
                 Image_id INT PRIMARY KEY,
                 Object BLOB,
                 name CHAR(50),
                 owner CHAR(20),
                 FOREIGN  KEY(owner) REFERENCES USERS(uname) ON DELETE CASCADE ON UPDATE CASCADE
    );""")

    conn.commit()
    conn.execute('PRAGMA foreign_keys = ON')
    return conn


from numpy.random import choice
from random import sample, random # get random group and user
from glob import glob
from user_group import *
from labeledImage import *


#Create some users
def feed_users(usernames, groupnames):

    group_c = len(groupnames)

    for gn in groupnames:
        usergr.addGroup(gn)
        gr_ins = usergr.getGroups(gn)
        print(gr_ins)

    for un in usernames:
        group_count = int(random() * (group_c)) # number of group that user belongs to
        users_group_list = sample(groupnames, group_count)# initialize with groups
        usergr.addUser(un, users_group_list, 'password123')
        us_ins = usergr.getUsers(un)


def feed_rule(image_shape):
    """
        # image_shape is needed to 
        # guarantee that randomly created shape did not cause any 
        # overflow 
    """
    global usernames
    global groupnames
    global regexps
    matchexpr_type = choice([1, 2, 3], p=[0.4, 0.2,
                                          0.4])  # 1 for USERNAME, 2 for REGEXP, 3 for  GROUPNAME note: regexp will be added later
    shape_type = choice([1, 2, 3], p=[0.3, 0.4, 0.3])  # 1 for RECTANGLE, 2 for POLYLINE, 3 for CIRCLE

    width, height, _ = image_shape  # channel is not needed

    if matchexpr_type == 1:
        rand_uname = choice(usernames)
        matchexpr = MatchExpr(matchexpr_type, rand_uname)
    elif matchexpr_type == 2:
        rand_expr = choice(regexps)
        matchexpr = MatchExpr(matchexpr_type, rand_expr)
    else:
        rand_groupname = choice(groupnames)
        matchexpr = MatchExpr(matchexpr_type, rand_groupname)

    if shape_type == 1:
        # create rectangle
        x1 = int(random() * (width / 2))
        x2 = int(random() * (width - x1)) + x1  # to guarantee x2 is above x1
        y1 = int(random() * (height / 2))
        y2 = int(random() * (height - y1)) + y1  # to guarantee y2 is above y1
        shape = Shape(shape_type, (x1, y1), (x2, y2))

    elif shape_type == 2:
        # create polyline
        #############################################################
        # not sure if it can create a proper shaped polylines?? # not most successful one but worked out
        line_number = int(random() * 50) + 3  # for simplicity
        x_l = [int(random() * (width / (line_number - i))) for i in range(line_number)]
        y_l = [int(random() * (height / (line_number - i))) for i in range(line_number)]
        points = list()
        for i in range(line_number):
            points.append((x_l[i], y_l[i]))  # polyline expects each argument is a tuple(x1, y1)
        shape = Shape(shape_type, *points)

    else:
        # create circle
        r = int(random() * (width / 2)) + 1  # not to cause overflow over image
        x1 = int(random() * width)
        y1 = int(random() * height)
        shape = Shape(shape_type, r, (x1, y1))

    return matchexpr, shape


def feed_images(image_path_list):
    # acts like randomly chosen user uploaded it and added rules
    # assumed users and groups already created
    im_name = 'image{}.jpg'
    im_instances = []
    global usernames
    for idx, path in enumerate(image_path_list):
        im_ins = LabeledImage() # empty constructors
        im_instances.append(im_ins)
        user = choice(usernames)
        im_ins.loadImage(path, user) # read image for given path
        rule_count = int(random() * 20) + 1 # at least one but not much than 15 for simplicity
        for i in range(rule_count):
            matchexpr, shape = feed_rule(im_ins.Image.shape)
            action_class = choice([ALLOW, DENY], p = [0.1, 0.9]) # blur is not implemented
            im_ins.addRule(matchexpr, shape, action_class)
        im_ins.save(im_name.format(idx))  # save it to persistent memory
    return im_instances


def CreateUsersAndGroups(users, groups):
        for group in groups:
            usergr.addGroup(group)
            
        for user in users:
            if 'boy' in user:
                usergr.addUser(user, ["above7", "men"], "changeme")
            elif 'girl' in user:
                usergr.addUser(user, ["above7", "women"], "changeme")
            elif 'teen' in user:
                usergr.addUser(user, ["above7", "above13"], "changeme")
            elif 'cia' in user:
                usergr.addUser(user, ["above7", "above13", "onlycia", "violence","women", "men"], "changeme")
            elif 'fbi' in user:
                usergr.addUser(user, ["above7", "above13", "onlyfbi", "violence","women", "men"], "changeme")
            elif '00' in user:
                usergr.addUser(user, ["violence", "onlycia", "onlyfbi", "topsecret", "above13", "women", "men", "above7"], "changeme")
            else:
                usergr.addUser(user, [], "changeme")

def gettingUsers(group):
    print(usergr.getUsers(group))
def gettingGroups(user):
    print(usergr.getGroups(user))
def membershipControl(user):
    for group in groupnames:
        if usergr.ismember(user, group):
            print("User '{}' is a member of group '{}'".format(user, group))
        else:
            print("User '{}' is NOT a member of group '{}'".format(user, group))

def testUserGroups():
    print("###Creating users and Groups")
    CreateUsersAndGroups(usernames, groupnames)

    print("##GETTING TOPSECRET USERS")
    gettingUsers("topsecret")
    print("##GETTING WRONG GROUP")
    gettingUsers("wronggroup")
    print("##GETTING CIA9900 GROUPS")
    gettingGroups("cia9900")
    print("##GETTING WRONG USER")
    gettingGroups("wrongusername")

    print("### MEMBERSHIP CONTROLS")
    membershipControl("cia9900")
    membershipControl("cia0000") #wrongname
    membershipControl("bossbaby01") #this user is not in any group

    print("## DELETED CIA9900")
    usergr.delUser("cia9900")
    print("##CIA9900'S GROUPS")
    gettingGroups("cia9900") #this user is deleted
    usergr.delGroup("topsecret")
    print("## TOPSECRET IS DELETED")
    gettingUsers("topsecret") #this group is deleted

    print("## JAMESBOND MEMBERHIPS")
    membershipControl("jamesbond007") #topsecret is deleted so the user shouldn't be member of it











if __name__ == '__main__':
    usernames = ['Veli', 'Hasan', 'Yesevi', 'Micheal', 'Excalibur',
                'Babayorgun', 'Lee', 'Nameless', 'Whoami',
                'Hayır', 'Evet', 'Var', 'Yok', 'Rooney',
                'Honey', 'Balım', 'Ayça26', 'Yoruk15', '15Baba15',
                "bossbaby01", "youtuberboy", "makeupgirlx", 
                "deathmetalteen", "nerdyteen", "populargirl007", 
                "desperatehousewife72", "president001", 
                "jamesbond007", "genera004", "fbi1385", 
                "fbi9843", "fbi4354", "cia9900", "cia1112", "cia4656",
                ]
    regexps =  [r'^[A-Z]+$',
                r'^[A-Z]+[a-z0-9]*$',
                r'^an[1-9]+$',
                r'^[A-Za-z]+le+[A-Za-z]*$',
               ]
    groupnames = ['Aslanlar', 'Kaplanlar', 'Askerler',
                'Asker kaçakları', 'Ultras',
                'Veliler', 'Students',
                'Qi', 'Bankers', 'Tankers',
                "violence", "onlycia", "onlyfbi", 
                "topsecret", "above13", "women", 
                "men", "above7"
                ]
    db_name = 'labeled_image.db'
    conn = construct_database(db_name)
    usergr = UserGroup(conn)
    LabeledImage.db_cursor = conn.cursor()

    feed_users(usernames, groupnames)

    image_path = 'images/'
    image_path_list = glob(image_path + '*.jpg')


    im_instances = feed_images(image_path_list)
    for im in im_instances:
        for username in usernames:
            image = im.getImage(username)
            cv2.imwrite('image/{}_[{}].jpg'.format(im.name[:-4], username), image)

    from_database_labeledImages = []

    ims_in_db = conn.execute('select name from LabeledImage').fetchall()

    for i in ims_in_db:
        ll = LabeledImage()
        ll.load(i[0])
        from_database_labeledImages.append(ll)
    # save it to local(since cv2.imshow does not work in package fetched via pip)
    for im in from_database_labeledImages:
        for user in usernames:
            image = im.getImage(user)
            cv2.imwrite('image/{}_[{}]_compare.jpg'.format(im.name[:-4], user), image)

    
    testUserGroups()






    usergr.addUser('Jessica', [], 'highsecuritypassword')

    usergr.addGroup('JessicaSevenlerDerneği')
    usergr.addUser('Justin', ['JessicaSevenlerDerneği',],'jessica')
    usergr.addUser('Helena', ['JessicaSevenlerDerneği',], 'nocomment')
    usergr.addUser('Belk00', ['JessicaSevenlerDerneği'], 'IloveNumbers')

    usergr.addGroup('JessicaHaters')
    usergr.addUser('DrVenom', ['JessicaHaters',], 'IhateJessica')
    usergr.addUser('Selena', ['JessicaHaters',], 'IdontloveJessica')
    usergr.addUser('TeenAgeGirl', ['JessicaHaters',], 'nocomment')
    usergr.addUser('UnderAgeBoy', ['JessicaHaters',], 'password')

    jbL = LabeledImage()
    jbL.loadImage('images/jb.jpg', 'Jessica')
    jbL.addRule(MatchExpr(MatchExpr.GROUPNAME, 'JessicaHaters'), Shape(Shape.CIRCLE, 50, (50, 100)), DENY)
    jbL.addRule(MatchExpr(MatchExpr.USERNAME, 'DrVenom'), Shape(Shape.POLYLINE, (50,50), (50, 75), (80, 100), (150, 160), (250, 400), (600, 650)), DENY)
    jbL.addRule(MatchExpr(MatchExpr.REGEXP, r'[A-Z][a-z]+[0-9]*'), Shape(Shape.CIRCLE, 400, (430, 430)), DENY)
    jbL.addRule(MatchExpr(MatchExpr.GROUPNAME, 'JessicaSevenlerDerneği'), Shape( Shape.RECTANGLE, (200, 200), (750, 750)), ALLOW)
    jbL.addRule(MatchExpr(MatchExpr.USERNAME, 'Justin'), Shape(Shape.RECTANGLE, (0,0), (870,870)), ALLOW)


