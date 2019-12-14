from django.forms import Form
from django.forms import FileField, TextInput

class ImageForm(Form):
    file = FileField()