class UserGroup:
    conn = None
    curs = None

    @staticmethod
    def addUser(name, groups, password):
        """
        If a group in the list does not exist in the database it prints that the groups is not in the groups database.
        """
        try:
            UserGroup.curs.execute("insert into Users values (?, ?)", (name, password))
            for group in groups:
                UserGroup.curs.execute("select * from groups where gname = ?", (group,))
                if UserGroup.curs.fetchone() is None:
                    return "The group {0} is not in the database!".format(group)
                    pass
                else:
                    UserGroup.curs.execute("insert into isMember values (?, ?)", (name, group))

            UserGroup.conn.commit()
        except Exception as e:
            if "UNIQUE constraint" in str(e):
                return "There is a user called '{}' already!".format(name)
            else:
                return str(e)
        return True

    @staticmethod
    def addGroup(name):
        """ Adds the group into groups table """
        try:
            UserGroup.curs.execute("insert into Groups values (?)", (name,))
            UserGroup.conn.commit()
        except Exception as e:
            if "UNIQUE constraint" in str(e):
                return "There is a group called '{}' already!".format(name)
            else:
                return (e)
        return True

    @staticmethod
    def delUser(name):
        """ Deletes the user from users also deletes the records in isMember """
        UserGroup.curs.execute("delete from Users where uname = ?", (name,))
        UserGroup.conn.commit()
        return True

    @staticmethod
    def delGroup(name):
        """ Deletes the group and the """
        UserGroup.curs.execute("delete from Groups where gname = ?", (name,))
        UserGroup.conn.commit()
        return True

    @staticmethod
    def getGroups(name):
        """ Getting list of the groups of the username"""
        UserGroup.curs.execute("select u.uname from Users u where uname=?", (name,))
        if UserGroup.curs.fetchone() is None:
            return "There is no user called '{}'".format(name)
        UserGroup.curs.execute("select m.gname from isMember m where m.uname = ?", (name,))
        fetched = UserGroup.curs.fetchall()
        if not fetched:
            return "There are no groups for the user '{}'".format(name)
        result = []
        for group in fetched:
            result.append(group[0])
            UserGroup.conn.commit()
        return result

    @staticmethod
    def getUsers(name):
        """ Getting list of the users of the group"""

        UserGroup.curs.execute("select g.gname from Groups g where gname=?", (name,))
        if UserGroup.curs.fetchone() is None:
            return "There is no group called '{}'".format(name)

        UserGroup.curs.execute("select m.uname from isMember m where m.gname = ?", (name,))
        fetched = UserGroup.curs.fetchall()
        if not fetched:
            return "There are no users for the group '{}'".format(name)
        result = []
        for group in fetched:
            result.append(group[0])
            UserGroup.conn.commit()
        return result

    @staticmethod
    def setPassword(user, password):
        UserGroup.curs.execute("select u.uname from Users u where uname=?", (user,))
        if UserGroup.curs.fetchone() is None:
            return "There is no user called '{}'".format(user)
        UserGroup.curs.execute("update Users set password = ? where uname = ?", (password, user))
        UserGroup.conn.commit()
        return True

    @staticmethod
    def ismember(user, group):
        UserGroup.curs.execute("select u.uname from Users u where uname=?", (user,))
        if UserGroup.curs.fetchone() is None:
            return "There is no user called '{}'".format(user)

        UserGroup.curs.execute("select g.gname from Groups g where gname=?", (group,))
        if UserGroup.curs.fetchone() is None:
            return "There is no group called '{}'".format(group)

        UserGroup.curs.execute("select * from isMember where uname = ? and gname = ?", (user, group))
        result = UserGroup.curs.fetchone()
        UserGroup.conn.commit()
        if result is None:
            return False
        else:
            return True

    @staticmethod
    def auth(user, passwd):
        UserGroup.curs.execute("select password from Users where uname = ?", (user, ))
        p = UserGroup.curs.fetchone()
        UserGroup.conn.commit()
        return p[0] == passwd

    def __del__(self):
        UserGroup.conn.close()
