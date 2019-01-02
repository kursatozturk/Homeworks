import sqlite3

DB_NAME = "ImgAnnTool.db"

conn = sqlite3.connect(DB_NAME)
c = conn.cursor()

def addUser(name, groups, password):
        """
        If a group in the list does not exist in the database it prints that the groups is not in the groups database.
        """
        try:
            c.execute("insert into Users values (?, ?)", (name, password))
            for group in groups:
                c.execute("select * from groups where gname = ?", (group,))
                if c.fetchone() is None:
                    print("The group {0} is not in the database!".format(group))
                    pass
                else:
                    c.execute("insert into isMember values (?, ?)", (name, group))

            conn.commit()
        except Exception as e:
            if "UNIQUE constraint" in str(e):
                print("There is a user called '{}' already!".format(name))
            else:
                print(e)

def addGroup(name):
    """ Adds the group into groups table """
    try:
        c.execute("insert into Groups values (?)", (name,))
        conn.commit()
    except Exception as e:
        if "UNIQUE constraint" in str(e):
            print("There is a group called '{}' already!".format(name))
        else:
            print(e)

# c.execute("""CREATE TABLE Users(
#             uname char(20),
#             password char(20),
#             PRIMARY KEY (uname)
#             );""")

# c.execute("""CREATE TABLE Groups (
#             gname char(20),
#             PRIMARY KEY (gname)
#             );""")

# c.execute("""CREATE TABLE isMember (
#             uname char(20),
#             gname char(20),
#             PRIMARY KEY(uname, gname)
#             FOREIGN KEY(uname) REFERENCES users ON DELETE CASCADE,
#             FOREIGN KEY(gname) REFERENCES groups ON DELETE CASCADE
#             );""")

# c.execute("""CREATE TABLE LabeledImage(
#                 Image_id INT PRIMARY KEY,
#                 Object BLOB,
#                 name CHAR(50),
#                 owner CHAR(20),
#                 FOREIGN  KEY(owner) REFERENCES USERS(uname) ON DELETE CASCADE ON UPDATE CASCADE
# );""")


usernames = ["bossbaby01", "youtuberboy", "makeupgirlx", 
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
groupnames = ["violence", "onlycia", "onlyfbi", 
            "topsecret", "above13", "women", 
            "men", "above7"
            ]


for group in groupnames:
    addGroup(group)
    
for user in usernames:
    if 'boy' in user:
        addUser(user, ["above7", "men"], "changeme")
    elif 'girl' in user:
        addUser(user, ["above7", "women"], "changeme")
    elif 'teen' in user:
        addUser(user, ["above7", "above13"], "changeme")
    elif 'cia' in user:
        addUser(user, ["above7", "above13", "onlycia", "violence","women", "men"], "changeme")
    elif 'fbi' in user:
        addUser(user, ["above7", "above13", "onlyfbi", "violence","women", "men"], "changeme")
    elif '00' in user:
        addUser(user, ["violence", "onlycia", "onlyfbi", "topsecret", "above13", "women", "men", "above7"], "changeme")
    else:
        addUser(user, [], "changeme")

conn.commit()
conn.close()
            