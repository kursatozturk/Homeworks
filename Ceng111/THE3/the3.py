from random import choice,randrange
from time import time

def hyphenate(s):
    Vowel=['a','e','i','I','u','U','o','O']
    a=[]
    d=0
    res=[]
    for i in s :
	if i=='.': res.append(''.join(a));a=[]
        a.append (i)
        if i in Vowel:
            d+=1
            if len (a)>1: 
                if a [-2] in Vowel:
                    res.append (''.join (a [:-1]))
                    a=a[-1:]
		    d=1
                    continue
            if len (a)>2:
                if a [-2] not in Vowel and d>1:
                    res.append (''.join (a [:-2]))
                    a=a [-2:]
                    d=1
		    continue    
    res.append (''.join (a))
    return res

def bighyph (s):
    d=s.split ()
    erase=[]
    holder=[]
    for i in range(len(d)):
	if str(d[i]).endswith('.'): continue
	if '.' in d[i]:
		xy=d[i].split('.')
		for y in xy:
			holder.append(y)
			holder.append('.')
		erase.append(i)
		del holder[-1]
    for i in reversed(erase):
	d=d[:int(i)]+holder+d[int(i+1):]
    a=[]
    erase=[]
    for i in d:
	if i=='.':a+=i;continue
	a.append(' ')
        a=a+hyphenate (i)
    if a [-1]==' ': del a [-1]
    return a   
 

def createdict (L):
    dic1={}
    a=[]
    dicstat={}
    for i in L:
        if i not in dic1: dic1.setdefault (i,{})
    for i in range (1,len (L)):
        a.append (1)
        if L [i] in (dic1 [L [i-1]]):
            dic1 [L [i-1]][L [i]]+=1
            continue
        dic1 [L [i-1]].setdefault (L [i],a [i-1])
    return dic1

  
def mostused_(dic1):
    tmp=0
    zaa=[]
    eraser=[]
    dic={}
    if len(dic1)>1: sorter=[['',0],['',0]]
    else: return dic1
    sorter=dic1.items()
    sorterkey1,sorterkey2='',''
    m,n=0,0
    for i in range(len(sorter)):
	if sorter[i][1]>m: 
		m=sorter[i][1]
		sorterkey1=sorter[i][0]
		tmp=i
    del sorter[tmp]
    
    for i in range(len(sorter)):
	if sorter[i][1]>n:
		n=sorter[i][1]
		sorterkey2=sorter[i][0]
    		tmp=i
    del sorter[tmp]
    for i in range(1,len(sorter)):
	if sorter[i][1]==m: 
		zaa.append(i)
    if zaa!=[]: dic={sorterkey1:m}
    else: dic= {sorterkey1:m,sorterkey2:n}
    if zaa!=[]:
	for i in zaa:
		dic.setdefault(sorter[i][0],sorter[i][1])
    return dic


    
def execute():
	a=raw_input()
	n,m=a.split()
	m=int(m)
	n=int(n)
	input_=[]
	L=[]
	k=0
	t=0
	z=0
	_text=''
	xxe=['']
	erase=[]
	listofsyllabes=[]
	twosyllabes={}
	while True:
		a=raw_input()
		if a=='=': break
		input_.append(a)
	for i in input_ :
		L.extend(bighyph(i))
	
        for i in range(len(L)-1):
		if L[i]=='.' and L[i+1]==' ': erase.append(i+1)
	for i in reversed(erase):
		del L[i]

	dicstattmp=createdict(L)
	for i in dicstattmp:
		twosyllabes.setdefault(i,mostused_(dicstattmp[i]))
	for i in twosyllabes:
		if twosyllabes[i]!=[]:
			listofsyllabes.append(twosyllabes[i].items())
	tempci=dicstattmp[' '].items()
	if '.' in dicstattmp:
		tempci.extend(dicstattmp['.'].items())
	xxe=choice(tempci)
	xx=xxe[0]
	while True:
		if z==1 and xx==' ': xsda=_text;z=2
		_text+= xx
		t+=1
		if z==2:_text=xsda
		if xx==' ': k+=1
		if xx=='.':
			_text+='\n'
			tempci=dicstattmp[' '].items()
			tempci.extend(dicstattmp['.'].items())
			xxe=choice(tempci)
			xx=xxe[0]
			k+=1
			z=1
			continue
		else: z=0
		tmplist=twosyllabes[xx].items()
		if tmplist==[]:break
		xx=choice(tmplist)[0]
		if (len(_text)>m or k>n) and (xx==' ' or xx=='.'): break
	print _text+xx
