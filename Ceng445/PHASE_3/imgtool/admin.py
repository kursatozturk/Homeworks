from django.contrib import admin

from .models import Group, ImgAnnToolUser, Image

admin.site.register(Group)
admin.site.register(ImgAnnToolUser)
admin.site.register(Image)

