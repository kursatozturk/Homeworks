def bool_(L,var,val):
	if L[0]=='not': return (not bool_(L[1],var,val))
	if L[0]=='==':
		if L[1] in var and L[2] not in var: return L[2]==val[var.index(L[1])]
		if L[2] in var and L[1] not in var: return L[1]==val[var.index(L[2])]
		if L[1] in var and L[2] in var: return val[var.index(L[1])] == val[var.index(L[2])]
		else: return L[1]==L[2]
	if L[0]=='!=':
		if L[1] in var and L[2] in var: return val[var.index(L[1])] != val[var.index(L[2])]
		elif L[1] in var: return L[2]!=val[var.index(L[1])]
		elif L[2] in var: return L[1]!=val[var.index(L[2])]
		else: return L[1]!=L[2]
	if L[0]=='<':
		if L[1] in var and L[2] in var: return val[var.index(L[1])] < val[var.index(L[2])]
		elif L[1] in var: return L[2]>val[var.index(L[1])]
		elif L[2] in var: return L[1]<val[var.index(L[2])]
		else: return L[1]<L[2]
	if L[0]=='>':
		if L[1] in var and L[2] in var: return val[var.index(L[1])] > val[var.index(L[2])]
		elif L[1] in var: return L[2]<val[var.index(L[1])]
		elif L[2] in var: return L[1]>val[var.index(L[2])]
		else: return L[1]>L[2]
	if L[0]=='in': return val[var.index(L[1])] in L[2]
	return True	
def orhelper(L,var,val):
	if L==[]:return False
	if L[0]=='and':return andhelper(L[0][1:],var,val) or orhelper(L[1:],var,val)
	if L[0]==[]:return orhelper(L[1:],var,val)
	if len(L)==1: return bool_(L[0],var,val)
	return orhelper(L[1:],var,val) or bool_(L[0],var,val)
def andhelper(L,var,val):
	if L[0]==[]:return False
	if L[0][0]=='or':return orhelper(L[0][1:],var,val) and andhepler(L[1:],var,val)
	if len(L)==1: return bool_(L[0],var,val)
	return andhelper(L[1:],var,val) and bool_(L[0],var,val)
def or_and(L,var,val):
	if len(L[1])<=1 and len(L[2])<=1:
		if L[0]=='and':
			return andhelper(L[1:],var,val)
		if L[0]=='or':
			return orhelper(L[1:],var,val)
	if L[0]=='and':
		return andhelper(L[1:],var,val)
	if L[0]=='or':
		return orhelper(L[1:],var,val)	
def query(decision_tree,variable_value):
	if type(decision_tree)!=list:
		return decision_tree
	variables=[]
	values=[]
	for item in variable_value:
		variables.append(item[0])
		values.append(item[1])
	if decision_tree[0][0]=='not':
		if decision_tree[0][1][0]=="and" or decision_tree[0][1][0]=='or':
			if or_and(decision_tree[0][1],variables,values): return query(decision_tree[2],variable_value)
			else: return query(decision_tree[1],variable_value)
	if decision_tree[0][0]=="and" or decision_tree[0][0]=='or':
		if or_and(decision_tree[0],variables,values): return query(decision_tree[1],variable_value)
		else: return query(decision_tree[2],variable_value)
	else:
		if bool_(decision_tree[0],variables,values): return query(decision_tree[1],variable_value)
		else: return query(decision_tree[2],variable_value)


