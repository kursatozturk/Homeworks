from builtins import bytearray

from .forms import BaseUserForm, ImgAnnToolUserForm, RuleForm, RuleDeletion, ActionForm
from .models import Group, Image, ImgAnnToolUser
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import get_object_or_404, render
from django.urls import reverse
from source.labeledImage import *


def pic_local_paths(im_list, username):
    paths = list()
    for im, im_name, owner in im_list:
        r_path = f'/tmp/{im_name}_{username}.jpg'
        cv2.imwrite('media' + r_path, im)
        paths.append((r_path, im_name, owner))
    return paths

    
def index(request):
    if request.user.is_anonymous:
        return HttpResponseRedirect(reverse('imgtool:login'))
    else:
        return HttpResponseRedirect(reverse('imgtool:home'))


@login_required(login_url='/login/')
def home(request):
    images = Image.objects.all()
    img_list = list()
    for im in images:
        name = im.name
        src = im.image_src
        owner = im.owner.user.username
        lbldIm = LabeledImage()
        lbldIm.load(src, name)
        img_list.append((lbldIm.getImage(request.user.username), name, owner))

    return render(request, 'imgtool/index.html', {'image_list': pic_local_paths(img_list, request.user.username)})

def register_user(request):
    
    if request.method == 'POST':

        user_form = BaseUserForm(request.POST)
        groups_form = ImgAnnToolUserForm(request.POST)
        if user_form.is_valid() and groups_form.is_valid():
            uname = user_form.cleaned_data['username']
            passwd = user_form.cleaned_data['password']
            user = User(username=uname)
            user.set_password(passwd)
            user.save()
            groups_chosen = groups_form.cleaned_data['groups']
            groups = [Group.objects.get(groupname=gr) for gr in groups_chosen]
            the_user = ImgAnnToolUser(user=user)
            the_user.save()
            the_user.groups.set(groups)
            the_user.save()
            user_l = authenticate(username=uname, password=passwd)
            if user_l is not None:
                login(request, user_l)
            return HttpResponseRedirect(reverse('imgtool:home'))
        else:
            return HttpResponse('INVALID FORM')
    

    bf = BaseUserForm()
    gf = ImgAnnToolUserForm()

    return render(request, 'imgtool/register.html', {'user_form': bf, 'groups_form': gf})

def login_user(request):
    """ Welcome Screen """
    
    if request.method == "POST":
        #login to 
        username = request.POST['username']
        passwd = request.POST['password']
        user = authenticate(username=username, password=passwd)
        if user is None:
            return render(request, 'imgtool/login.html', {'msg': 'Wrong credientials!'})
        else:
            login(request, user)
            return HttpResponseRedirect(reverse('imgtool:home'))
    return render(request, 'imgtool/login.html', {'msg': ''})

@login_required(login_url='/login/')
def user(request, username):
    """ User Page """
    if request.method == 'POST':
        passwd = request.POST['password']
        uname = request.user.username
        userl = authenticate(username=uname, password=passwd)
        if userl: # authenticated!
            groups_form = ImgAnnToolUserForm(request.POST)
            if groups_form.is_valid():
                groups_chosen = groups_form.cleaned_data['groups']
                groups = [Group.objects.get(groupname=gr) for gr in groups_chosen]
                user = userl.user
                user.groups.set(groups)
                user.save()
                return HttpResponseRedirect(reverse('imgtool:home'))

    if username == request.user.username:
        user = request.user
        self_page = True
    else:
        user = User.objects.get(username=username)
        self_page = False

    the_user = ImgAnnToolUser.objects.get(user=user)
    groups = the_user.groups.all()
    images = Image.objects.filter(owner=the_user)
    img_list = list()
    for im in images:
        name = im.name
        src = im.image_src
        owner = im.owner.user.username
        lbldIm = LabeledImage()
        lbldIm.load(src, name)
        img_list.append((lbldIm.getImage(request.user.username), name, owner))
    if self_page:
        form = ImgAnnToolUserForm(initial={'groups': groups})
    else:
        form = None
    return render(request, 'imgtool/user.html', {'groups': groups, 
                'image_list': pic_local_paths(img_list, request.user.username), 
                'groupform': form,
                'self_page': self_page})

@login_required(login_url='/login/')
def set_password(request, username):
    """ Only the user can change his/her password AUTH IS NOT APPLIED YET"""
    if request.user.username != username:
        return HttpResponse('You cannot change another user\'s password!')
    if request.method == 'POST':
        password: str = request.POST['password']
        the_user: User = User.objects.get(username=username)
        the_user.set_password(password) 
        the_user.save()
        u1 = authenticate(username=username, password=password)
        if u1 is not None:
            login(request, u1)
            return HttpResponseRedirect(reverse('imgtool:home'))
        else:
            return HttpResponse('Something went wrong')
    else:
        return render(request, 'imgtool/set_password.html')
        
@login_required(login_url='/login/')
def add_image(request):

    if request.method == 'POST':
        """
        img_form = ImageForm(request.POST)
        if img_form.is_valid():
            name = img_form.cleaned_data['image_name']
            img = img_form.cleaned_data['image_src']
            user = request.user.user
            lbldIm = LabeledImage(user=user)
            lbldIm.set_image_directly(img)
            obj = lbldIm.save(name)
            img_model = Image(image_src=obj, name=name, owner=user)
            img_model.save()
            return HttpResponseRedirect(f'/image/{name}/add_rule/')
        else:
            print('Form is not Valid!')
        """
        name = request.POST['name']
        image = request.FILES['image'].read()
        user = request.user
        the_user = ImgAnnToolUser.objects.get(user=user)
        lbldIm = LabeledImage()
        lbldIm.setImage(image)
        obj = lbldIm.save(name)
        img_model = Image(image_src=obj, name=name, owner=the_user)
        img_model.save()
        return HttpResponseRedirect(reverse('imgtool:rulepage', args=(name, )))


    return render(request, 'imgtool/add_image.html')

@login_required(login_url='/login/')
def image(request, image_name):
    """ Get Image """
    
    image = Image.objects.get(name=image_name)
    
    if image:
        lbldIm = LabeledImage()
        lbldIm.load(image.image_src,name=image_name)
        img = lbldIm.getImage(request.user.username)
        paths = pic_local_paths([(img, image_name, image.owner.user.username) ], request.user.username)
        return render(request, 'imgtool/image.html', {'path_img': paths[0][0], 'name': paths[0][1], 'owner': paths[0][2]   })
    else:
        return HttpResponse('IMAGE DOES NOT EXISTS')    

@login_required(login_url='/login/')
def add_rule(request, image_name):
    img = Image.objects.get(name=image_name)
    if img.owner.user != request.user:
        return HttpResponse('You are not authorized to add rule to this image!')
    if request.method == 'POST':
        rules = RuleForm(request.POST)
        if rules.is_valid():
            shape = rules.cleaned_data['shape']
            mexpr_s = rules.cleaned_data['match_expr']
            action_s = rules.cleaned_data['action']
            points = rules.cleaned_data['points'].split(',')
            string = '(' + shape + ', '
            if shape == 'CIRCLE':
                assert(len(points) == 3) # r, x0, y0
                string += ','.join(points) + ')'
            elif shape == 'RECTANGLE':
                assert(len(points) == 4)# x0, y0, x1, y1
                string += ','.join(points) + ')'
            elif shape == 'POLYLINE':
                assert(len(points) % 2 == 0) # there exists y_n for each x_n
                string += '[ ' + ','.join([f'({points[i]}, {points[i+1]})' for i in range(0, len(points), 2)]) + '])'
            shape_shape = Shape(shape_string=string)
            match_expr = MatchExpr(mexpr_s)
            action = ALLOW if action_s == 'ALLOW' else (DENY if action_s == 'DENY' else BLUR)
            image = Image.objects.get(name = image_name)
            obj = image.image_src
            lbldIm = LabeledImage()
            lbldIm.load(obj, image_name)
            lbldIm.addRule(matchexpr=match_expr, shape=shape_shape, action=action)
            image.image_src = lbldIm.save(image_name)
            image.save()
            return HttpResponseRedirect(reverse('imgtool:rulepage', args=(image_name, )))
        else:
            return HttpResponse('INVALID FORM')
    rule_form = RuleForm()
    return render(request, 'imgtool/add_rule.html', {'rule_form': rule_form})

@login_required(login_url='/login')
def del_rule(request, image_name):

    try:
        img = Image.objects.get(name=image_name)
    except:
        return HttpResponse('IMAGE NOT FOUND!')
    lbldIm = LabeledImage()
    lbldIm.load(img.image_src,name=image_name)
    rules = lbldIm.Rules
    action = {ALLOW : 'ALLOW', DENY: 'DENY', BLUR: 'BLUR'}
    rules = [(x, y, action[z]) for x,y,z in rules]
    if request.method == 'POST':
        r_form = RuleDeletion(rules, request.POST)
        if r_form.is_valid():
            rule_idxs = r_form.cleaned_data.get('rules')
            obj = img.image_src
            lbl = LabeledImage()
            lbl.load(obj, image_name)
            rule_idxs.reverse()
            for rule_idx in rule_idxs:
                lbl.delRule(int(rule_idx))
            obj = lbl.save(image_name)
            img.image_src = obj
            img.save()
            return HttpResponseRedirect(reverse('imgtool:home'))
        else:
            return HttpResponse('Something Went Wrong!')
    
    rule_form = RuleDeletion(all_rules=rules)
    owner = img.owner.user
    if owner != request.user:
        return HttpResponse('You are not authorized to delete rule for this image!')
    return render(request, 'imgtool/del_rule.html', {'rule_form': rule_form})

@login_required(login_url='/login/')
def change_def_action(request, image_name):
    img = Image.objects.get(name=image_name)
    if img.owner.user != request.user:
        return HttpResponse('You are not authorized to add rule to this image!')
    if request.method == 'POST':
        act_form = ActionForm(request.POST)
        if act_form.is_valid():
            default_action = act_form.cleaned_data.get('default_action')        
            lbldIm = LabeledImage()
            lbldIm.load(img.image_src,name=image_name)
            def_action = ALLOW if default_action == 'ALLOW' else (DENY if default_action == 'DENY' else BLUR)
            lbldIm.setDefault(def_action)
            img.image_src = lbldIm.save(name=image_name)
            img.save()
            return HttpResponseRedirect(reverse('imgtool:home'))
        else:
            return HttpResponse('INVALID FORM')
    act_form = ActionForm()
    return render(request, 'imgtool/action_page.html', {'form': act_form})
    
@login_required(login_url='/login/')
def del_image(request, image_name):
    img = Image.objects.get(name=image_name)
    if img.owner.user != request.user:
        return HttpResponse('You are not authorized to add rule to this image!')
    Image.objects.filter(name=image_name).delete()
    return HttpResponseRedirect(reverse('imgtool:home'))

@login_required(login_url='/login')
def logout_user(request):
    logout(request)
    return HttpResponseRedirect(reverse('imgtool:home'))
