from imgtool.models import User, Group
from django.core.exceptions import ObjectDoesNotExist
from imgtool.models import ImgAnnToolUser



class UserGroup:
    conn = None
    curs = None
    #TODO: make proper changes to sync this' db with django's
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
    def getGroups(username):
        user = User.objects.get(username=username)
        return user.groups.all()

    @staticmethod
    def getUsers(groupname):
        group = Group.objects.get(groupname=groupname)
        return group.members.all()

    @staticmethod
    def setPassword(user, password):
        UserGroup.curs.execute("select u.uname from Users u where uname=?", (user,))
        if UserGroup.curs.fetchone() is None:
            return "There is no user called '{}'".format(user)
        UserGroup.curs.execute("update Users set password = ? where uname = ?", (password, user))
        UserGroup.conn.commit()
        return True

    @staticmethod
    def is_member(username, groupname):
        try:
            user = ImgAnnToolUser(user=User.objects.get(username=username))
        except ObjectDoesNotExist:
            print('user does not exists')
            return False
        try:
            group = Group.objects.get(groupname=groupname)
            for us in group.members.all():
                if us.user.username == user.user.username:
                    return True
            return False
        except ObjectDoesNotExist:
            print(f'such group named as {groupname} does not exists')
            return False

    @staticmethod
    def auth(user, passwd):
        UserGroup.curs.execute("select password from Users where uname = ?", (user, ))
        p = UserGroup.curs.fetchone()
        UserGroup.conn.commit()
        return p[0] == passwd

    def __del__(self):
        UserGroup.conn.close()
