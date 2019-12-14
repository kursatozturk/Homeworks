import socket
import pickle
import cv2
import sys


class Client:
    port_number = 1026
    sock = None
    logged_in = False
    user = None
    meta_data = None

    @staticmethod
    def show_possible_acts():
        print('You can choose one of the above:')
        for key in Client.meta_data.keys():
            print(key, end = ' ')

    @staticmethod
    def start():
        Client.sock = socket.socket()
        Client.sock.connect((socket.gethostname(), Client.port_number))
        Client.logged_in = False
        Client.user = None
        Client.meta_data = {
            'login': Client.login,
            'register': Client.register,
        }
        while True:
            Client.show_possible_acts()
            act = input('\nAction: ')
            if act in Client.meta_data:
                Client.meta_data[act]()

    @staticmethod
    def register():
        usname = input('choose a username: ')
        groups = input('Give group names(sepearte with comma(,), if no user, just push enter:\n')
        passwd = input('choose a password: ')  # ignored safety issues
        groups = [] if groups == '' else groups.split(',')
        data = {
            'func': 'add_user',
            'args': (usname, groups, passwd),
        }
        resp = _Client(Client.sock, 'admin').run(data)
        if resp is True:
            print(' Succesfully registered. Now you can log in!')
        elif type(resp) is str:
            print(resp)
        else:
            print('You can\'t register, Sorry!')

    @staticmethod
    def login():
        usname = input('username: ')
        passwd = input('password: ')  # ignored safety issues
        data = {
            'func': 'auth',
            'args': (passwd,),
        }
        resp = _Client(Client.sock, usname).run(data)
        if resp is True:
            Client.logged_in = True
            Client.user = usname
            meta_data = {
                'set_password': Client.set_password,
                'get_image': Client.get_image,
                'upload_image': Client.upload_image,
                'set_default': Client.set_default,
                'add_rule': Client.add_rule,
                'del_rule': Client.del_rule,
                'logout': Client.logout,
            }
            if Client.user == 'admin':
                admin_data = {
                    'add_user': Client.add_user,
                    'add_group': Client.add_group,
                    'is_member': Client.is_member,
                    'del_user': Client.del_user,
                    'del_group': Client.del_group,
                    'get_groups': Client.get_groups,
                    'get_users': Client.get_users,
                }
                Client.meta_data = dict(**meta_data, **admin_data)
            else:
                Client.meta_data = meta_data
            print('Successfully logged in!')
        else:
            print(resp)

    @staticmethod
    def logout():
        # TODO: Delete all data
        Client.logged_in = False
        #Client.sock.close()
        Client.meta_data = {
            'login': Client.login,
            'register': Client.register,
        }

    @staticmethod
    def is_member():
        uname = input('username: ')
        gname = input('group name: ')
        data = {
            'func': 'is_member',
            'args': (uname, gname)
        }
        resp = _Client(Client.sock, Client.user).run(data)

        if type(resp) is str:
            print(resp)
        elif resp is True:
            print('Yes')
        else:
            print('No')

    @staticmethod
    def set_default():
        img_name = input('give image name: ')
        default_act = input('give new default action: ')
        data = {
            'func': 'set_default',
            'args': (img_name, default_act,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Default changed successfully!')

    @staticmethod
    def del_rule():
        pos = input('Position of rule to be deleted: ')
        data = {
            'func': 'del_rule',
            'args': (pos,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Rule deleted successfully!')

    @staticmethod
    def set_password():
        new_passwd = input('Give new password: ')  # safety ignored here
        data = {
            'func': 'set_password',
            'args': (new_passwd,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Password changed successfully!')

    @staticmethod
    def get_users():
        gname = input('give group name: ')
        data = {
            'func': 'get_users',
            'args': (gname,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        for res in resp:
            print(res, end=' ')
        print('\n')

    @staticmethod
    def get_groups():
        uname = input('give username: ')
        data = {
            'func': 'get_groups',
            'args': (uname,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        for res in resp:
            print(res, end=' ')
        print('\n')

    @staticmethod
    def del_group():
        group_name = input('give group name: ')
        data = {
            'func': 'del_group',
            'args': (group_name,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Group deleted successfully!')
        elif type(resp) is str:
            print(resp)

    @staticmethod
    def del_user():
        username = input('give user name: ')
        data = {
            'func': 'del_user',
            'args': (username,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('User deleted successfully!')
        elif type(resp) is str:
            print(resp)

    @staticmethod
    def add_user():
        username = input('give user name: ')
        groups = input('Give group names(sepearte with comma(,), if no user, just push enter:\n')
        password = input('give password: ')
        groups = [] if groups == '' else groups.split(',')
        data = {
            'func': 'add_user',
            'args': (username, groups, password)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('User added successfully!')
        elif type(resp) is str:
            print(resp)

    @staticmethod
    def add_group():
        group_name = input('give group name: ')
        data = {
            'func': 'add_group',
            'args': (group_name,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Group added successfully!')
        elif type(resp) is str:
            print(resp)

    @staticmethod
    def add_rule():
        img_name = input('Which image?')
        match_expr = input('Match Expr: ')
        shape = input('shape: ')
        action = input('Action (allow, deny, blur) : ')
        pos = int(input('Position of rule(-1 to append end of the list)'))
        data = {
            'func': 'add_rule',
            'args': (img_name, match_expr, shape, action, pos),
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Rule added successfully!')
        elif type(resp) is str:
            print(resp)

    @staticmethod
    def upload_image():
        img_name = input('Please give a name to it: ')
        path = input('Please provide path to image: ')
        data = {
            'func': 'upload_from_file',
            'args': (img_name, path,),
        }
        resp = _Client(Client.sock, Client.user).run(data)
        if resp is True:
            print('Image successfully uploaded.')

    @staticmethod
    def get_image():
        img_name = input('Please specify the image name: ')
        data = {
            'func': 'get_image',
            'args': (img_name,)
        }
        resp = _Client(Client.sock, Client.user).run(data)
        print('Due to the limitations of OpenCV on linux, we shall show image with writing to local storage.')
        try:
            cv2.imwrite('{}_for_{}.jpg'.format(img_name, Client.user), resp)
        except TypeError:
            print('It seems there is no image named {}'.format(img_name))

class _Client:
    def __init__(self, sock, username):
        self.username = username
        self.sock = sock

    def run(self, data):
        data["client_name"] = self.username
        dumped_data = pickle.dumps(data)
        size = dumped_data.__sizeof__()
        buffer_size = size.to_bytes(4, sys.byteorder)
        self.sock.send(buffer_size + dumped_data)

        buffer_size = int.from_bytes(self.sock.recv(4), sys.byteorder)
        resp = self.sock.recv(buffer_size)
        resp = pickle.loads(resp)
        return resp


if __name__ == '__main__':
    Client.start()
