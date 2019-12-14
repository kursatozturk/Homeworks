
from django import forms
from django.contrib.auth.models import User
from .models import ImgAnnToolUser, Group, Image




#TODO: write form for each model 
class BaseUserForm(forms.Form):
    username = forms.CharField(max_length=100)
    password = forms.CharField(widget=forms.PasswordInput)

class ImgAnnForm(forms.Form):
    image = forms.ImageField(required=True)
    name = forms.CharField(max_length=200)

class RuleDeletion(forms.Form):
    rules =forms.MultipleChoiceField(required=False, widget=forms.CheckboxSelectMultiple)
    def __init__(self, all_rules, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fields['rules'].choices = [(idx, str(i)) for idx, i in enumerate(all_rules)]
    
class ImgAnnToolUserForm(forms.ModelForm):
    groups = forms.MultipleChoiceField(required=False, widget=forms.CheckboxSelectMultiple)
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fields['groups'].choices = [(i, i) for i in Group.objects.all()]

    class Meta:
        exclude = ['user',]
        model = ImgAnnToolUser

class ActionForm(forms.Form):
    default_action = forms.ChoiceField(choices=[('ALOW', 'ALLOW'), ('DENY', 'DENY'), ('BLUR', 'BLUR')], required=True)

class RuleForm(forms.Form):
    shape = forms.ChoiceField(choices=[('CIRCLE', 'CIRCLE'), ('RECTANGLE', 'RECTANGLE'), ('POLYLINE', 'POLYLINE')], required=True)
    match_expr = forms.CharField(max_length=200, required=True)
    action = forms.ChoiceField(choices=[('ALLOW','ALLOW'), ('DENY', 'DENY'), ('BLUR', 'BLUR')], required=True)
    points = forms.CharField(max_length=10000, required=True)