from django.shortcuts import render
from django.http import JsonResponse, HttpResponse
from requests import post, get
from .forms import ImageForm
import json
# Create your views here.
def index(request):
    return render(request, 'kingofqr/home.html')

def qrReader(request):
    data = {'data': ''}
    if request.method == 'POST':
        mxfs = request.POST['MAX_FILE_SIZE']
        img = request.FILES['file']
        b = b''
        for d in img:
            print(d, end='___' *25 +  '\n' * 2)
            b += d
        print(b)
        params = {
            'MAX_FILE_SIZE': mxfs,
            'file': b,
        }
        file = {
            'file': b,
        }
        r = post('http://api.qrserver.com/v1/read-qr-code/', files=file)
        if r.status_code == 200:
            response = r.json()[0]
            data['data'] = response['symbol'][0]['data']
    return JsonResponse(data=data)
def qrCreator(request):
    img = None
    print(request.POST)
    url = ''
    if request.method == 'POST':
        """
        data = request.POST.get('data')
        color = request.POST.get('color')
        bgcolor = request.POST.get('bgcolor')
        params = {}
        if not data:
            return JsonResponse(data={'ERROR': 'NO DATA PROVIDED!!'})
        params['data'] = data
        if color:
            params['color'] = color
        if bgcolor:
            params['bgcolor'] = bgcolor
        r = get('http://api.qrserver.com/v1/create-qr-code/', params=params)
        print(r)
        if r.status_code == 200:
            print(r.content)
            img = r.content
        """
        data = request.POST.get('data')
        color = request.POST.get('color')
        bgcolor = request.POST.get('bgcolor')
        params = {}
        if not data:
            return JsonResponse(data={'ERROR': 'NO DATA PROVIDED!!'})
        params['data'] = data
        if color:
            params['color'] = color

        if bgcolor:
            params['bgcolor'] = bgcolor
        url = 'http://api.qrserver.com/v1/create-qr-code/?'
        for key, value in params.items():
            url += f'&{key}={value}'


    return HttpResponse(content=url)
        