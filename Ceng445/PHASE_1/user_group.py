import sqlite3


class UserGroup():

    __conn = None
    __c = None

    def __init__(self, conn):
        self.__conn = conn
        self.__c = self.__conn.cursor()

    def addUser(self, name, groups, password):
        """
        If a group in the list does not exist in the database it prints that the groups is not in the groups database.
        """
        try:
            self.__c.execute("insert into Users values (?, ?)", (name, password)) 
            for group in groups:
                self.__c.execute("select * from groups where gname = ?", (group,))
                if self.__c.fetchone() == None:
                    print("The group {0} is not in the database!".format(group))
                    pass
                else:
                    self.__c.execute("insert into isMember values (?, ?)", (name, group))
            self.__conn.commit()
        except Exception as e:
            if "UNIQUE constraint" in str(e):
                print("There is a user called '{}' already!".format(name))
            else:
                print(e)

    def addGroup(self, name):
        """ Adds the group into groups table """
        try:
            self.__c.execute("insert into Groups values (?)", (name,))
            self.__conn.commit()
        except Exception as e:
            if "UNIQUE constraint" in str(e):
                print("There is a group called '{}' already!".format(name))
            else:
                print(e)

    def delUser(self, name):
        """ Deletes the user from users also deletes the records in isMember """
        self.__c.execute("delete from Users where uname = ?", (name,))
        self.__conn.commit()

    def delGroup(self, name):
        """ Deletes the group and the """
        self.__c.execute("delete from Groups where gname = ?", (name,))
        self.__conn.commit()

    def getGroups(self, name):
        """ Getting list of the groups of the username"""
        self.__c.execute("select u.uname from Users u where uname=?", (name,))
        if self.__c.fetchone() == None:
            return "There is no user called '{}'".format(name)
        self.__c.execute("select m.gname from isMember m where m.uname = ?", (name,))
        fetched = self.__c.fetchall()
        if not fetched:
            return "There are no groups for the user '{}'".format(name)
        result = []
        for group in fetched:
            result.append(group[0])
        self.__conn.commit()
        return result

    def getUsers(self, name):
        """ Getting list of the users of the group"""

        self.__c.execute("select g.gname from Groups g where gname=?", (name,))
        if self.__c.fetchone() == None:
            print("There is no group called '{}'".format(name))
            return []
        self.__c.execute("select m.uname from isMember m where m.gname = ?", (name,))
        fetched = self.__c.fetchall()
        if not fetched:
            print("There are no users for the group '{}'".format(name))
            return []
        result = []
        for group in fetched:
            result.append(group[0])
        self.__conn.commit()
        return result


    def setPassword(self, user, password):  
        self.__c.execute("select u.uname from Users u where uname=?", (user,))
        if self.__c.fetchone() == None:
            return "There is no user called '{}'".format(user)
        self.__c.execute("update Users set password = ? where uname = ?", (password, user))
        self.__conn.commit()

    def ismember(self, user, group):
        self.__c.execute("select u.uname from Users u where uname=?", (user,))
        if self.__c.fetchone() == None:
            print("There is no user called '{}'".format(user))
            return False
        self.__c.execute("select g.gname from Groups g where gname=?", (group,))
        if self.__c.fetchone() == None:
            print("There is no group called '{}'".format(group))
            return False
        self.__c.execute("select * from isMember where uname = ? and gname = ?", (user, group))
        result = self.__c.fetchone()
        self.__conn.commit()
        if result == None:
            return False
        else:
            return True

    def __del__(self):
        self.__conn.close()