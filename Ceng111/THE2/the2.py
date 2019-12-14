sum=i=k=m=s=temp=tmp=0;result='';IDch='';IDch=raw_input();a=len(IDch)
if a!=8: print('INVALID');exit()
for s in range(0,a):
	if IDch[s]=='?':
		if s==a-1:
			for i in range(0,a-2):
				if i%2==0:
					sum+=int(IDch[i])
				if i%2!=0:
					tmp=2*int(IDch[i])
					if tmp>=10:	
						tmpp=str(tmp)
						tmp=int(tmpp[0])+int(tmpp[1])
					sum+=tmp	
			while sum%10!=0:
				sum+=1
				k+=1
			tempz=list(IDch)
			tempz[a-1]=str(k)
			result=''.join(tempz)
			print(result)
			exit()
		if s%2!=0:
			for m in range(0,s):
				if m%2!=0:
					tmp=2*int(IDch[m])
					if tmp>=10:
						tmpp=str(tmp)
						tmp=int(tmpp[0])+int(tmpp[1])
					sum+=tmp
				if m%2==0:
					sum+=int(IDch[m])
			m=s+1
			for m in range(s+1,a-2):
				if m%2!=0:
					tmp=2*int(IDch[m])
					if tmp>=10:
						tmpp=str(tmp)
						tmp=int(tmpp[0])+int(tmpp[1])
					sum+=tmp
				if m%2==0:
					sum+=int(IDch[m])
			while sum%10!=0:
				sum+=1
				k+=1	
			temp=(k-int(IDch[a-1]))
			if temp<0: temp+=10
			if temp%2!=0: temp+=9
			temp/=2
			tempz=list(IDch)
			tempz[s]=str(temp)
			result=''.join(tempz)
			print(result)
			exit()	
		for m in range(0,s):
			if m%2!=0:
				tmp=2*int(IDch[m])
				if tmp>=10:						
					tmpp=str(tmp)
					tmp=int(tmpp[0])+int(tmpp[1])
				sum+=tmp
			if m%2==0:
				sum+=int(IDch[m])
		m=s+1
		for m in range(s+1,a-2):
			if m%2!=0:
				tmp=2*int(IDch[m])
				if tmp>=10:		
					tmpp=str(tmp)
					tmp=int(tmpp[0])+int(tmpp[1])
				sum+=tmp
			if m%2==0:
				sum+=int(IDch[m])
		while sum%10!=0:
			sum+=1
			k+=1	
		temp=k-int(IDch[a-1])
		if temp<0:
			temp+=10
		tempz=list(IDch)
		tempz[s]=str(temp)
		result=''.join(tempz)
		print(result)
		exit()	
for i in range(0,a-2):
	if i%2==0:
		sum+=int(IDch[i])
	if i%2!=0:
		tmp=2*int(IDch[i])
		if tmp>=10:
			tmpp=str(tmp)
			tmp=int(tmpp[0])+int(tmpp[1])
		sum+=tmp	
while sum%10!=0:
	sum+=1
	k+=1
if k==int(IDch[a-1]):
	print('VALID')
	exit()
else:
	print('INVALID')
	exit()
