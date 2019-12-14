from socket import *
from threading import Lock
import sqlite3
from labeledImage import *
from user_group import UserGroup
from concurrent.futures import ThreadPoolExecutor as TPExec
import sys


class ImgAnnTool:
    @staticmethod
    def ImageList(conn):
        c = conn.cursor()
        c.execute("select i.owner, i.name from LabeledImage")
        fetched = c.fetchall()
        result = []
        if not fetched:
            print("There is no image in the database")
            return result
        for group in fetched:
            result.append((group[0], group[1]))
        conn.commit()
        conn.close()
        return result

    @staticmethod
    def getImage(user, img_name):
        img = LabeledImage()
        img.load(img_name)
        return img.getImage(user)

    @staticmethod
    def upload_from_file(user, img_name, img_path):
        try:
            img = LabeledImage(user=user)
            img.loadImage(img_path)
            img.save(img_name)
            return True
        except:
            return False
        return True

    @staticmethod
    def uploadImage(user, img_buffer, img_name):
        try:
            img = LabeledImage(user=user)
            img.setImage(img_buffer)
            img.save(img_name)
            return True
        except:
            return False

    @staticmethod
    def add_rule(user, img_name, match_expr, shape, action, pos):
        img = LabeledImage()
        img.load(img_name)
        owner = img.owner
        if owner != user:
            return 'User is not the owner of image!'

        a = ALLOW if action == 'allow' else (DENY if action == 'deny' else (BLUR if action == 'blur' else None))
        if a is None:
            return 'Action does not understood'
        img.addRule(MatchExpr(match_expr), Shape(shape), a, int(pos))
        img.save(img_name)
        return True

    @staticmethod
    def add_group(user, group_name):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.addGroup(group_name)

    @staticmethod
    def add_user(user, user_name, groups: [str], passwd):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.addUser(user_name, groups, passwd)

    @staticmethod
    def auth_user(user, passwd):
        return UserGroup.auth(user, passwd)

    @staticmethod
    def del_user(user, user_name):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.delUser(user_name)

    @staticmethod
    def del_group(user, group_name):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.delGroup(group_name)

    @staticmethod
    def get_groups(user, user_name):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.getGroups(user_name)

    @staticmethod
    def get_users(user, group_name):
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.getUsers(group_name)

    @staticmethod
    def set_passwd(user, passwd):
        return UserGroup.setPassword(user, passwd)

    @staticmethod
    def del_rule(user, img_name, pos):
        img = LabeledImage()
        img.load(img_name)
        if user != img.owner:
            return 'User is not the owner of image!'
        img.delRule(pos)
        img.save(img_name)
        return True

    @staticmethod
    def set_default(user, image_name, action):
        img = LabeledImage()
        img.load(image_name)
        if user != img.owner:
            return 'User is not the owner of image!'

        a = ALLOW if action == 'allow' else (DENY if action == 'deny' else (BLUR if action == 'blur' else None))
        if a is None:
            return 'Action does not understood'
        img.setDefault(a)
        img.save(image_name)
        return True

    @staticmethod
    def is_member(user, user_name, group_name):  # IS MEMBER EKLEDIM ~makca
        if user != 'admin':
            return 'this requires admin privileges!'
        return UserGroup.ismember(user_name, group_name)


class Server:
    meta_data = {
        'get_image': ImgAnnTool.getImage,
        'upload_image': ImgAnnTool.uploadImage,
        'upload_from_file': ImgAnnTool.upload_from_file,
        'add_rule': ImgAnnTool.add_rule,
        'add_group': ImgAnnTool.add_group,
        'add_user': ImgAnnTool.add_user,
        'del_user': ImgAnnTool.del_user,
        'del_group': ImgAnnTool.del_group,
        'get_groups': ImgAnnTool.get_groups,
        'get_users': ImgAnnTool.get_users,
        'set_password': ImgAnnTool.set_passwd,
        'del_rule': ImgAnnTool.del_rule,
        'set_default': ImgAnnTool.set_default,
        'is_member': ImgAnnTool.is_member,
        'auth': ImgAnnTool.auth_user,
    }

    db_lock = Lock()

    @staticmethod
    def select_func(func_name):
        return Server.meta_data[func_name]

    @staticmethod
    def connection_handler(recv_sock, addr):
        print('{} is accepted by connection handler'.format(addr))
        while True:
            try:
                buffer_size = recv_sock.recv(4)  # Client sends data length, in first 4 bytes
                if buffer_size == b'':
                    print('Connection closed by Client addr: {}'.format(addr))
                    break
                buffer_size = int.from_bytes(buffer_size, sys.byteorder)
                data = recv_sock.recv(buffer_size)
                resp_dict = pickle.loads(data)  # type: dict
                try:
                    user = resp_dict['client_name']
                    func = resp_dict['func']
                    args = resp_dict['args']
                    func = Server.select_func(func)
                    # lock db
                    Server.db_lock.acquire()
                    db = sqlite3.connect('ImgAnnTool.db')
                    db.execute("PRAGMA foreign_keys=ON")
                    UserGroup.conn = db
                    UserGroup.curs = db.cursor()
                    LabeledImage.db_cursor = db.cursor()
                    try:
                        resp = pickle.dumps(func(user, *args))
                    except TypeError as e:
                        print('Type Error', e)
                        resp = pickle.dumps(str(e))
                    db.commit()
                    db.close()
                    Server.db_lock.release()
                except KeyError as e:
                    print('Key Error')
                    resp = pickle.dumps(str(e))

            except Exception as e:
                print('any exception: {}'.format(e))
                resp = pickle.dumps(str(e))
            size = resp.__sizeof__()
            size = size.to_bytes(4, sys.byteorder)
            recv_sock.send(size + resp)
        recv_sock.close()

    @staticmethod
    def start():
        with TPExec() as pool:
            with socket(AF_INET, SOCK_STREAM) as sock:
                sock.bind(('', 1026))
                sock.listen(10)
                while True:
                    ns, addr = sock.accept()
                    pool.submit(Server.connection_handler, ns, addr)


if __name__ == '__main__':
    Server.start()
