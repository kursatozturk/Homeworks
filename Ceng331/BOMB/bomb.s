
bomb:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	callq  *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	retq   

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 e2 2f 00 00    	pushq  0x2fe2(%rip)        # 4008 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 e4 2f 00 00    	jmpq   *0x2fe4(%rip)        # 4010 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <__stack_chk_fail@plt>:
    1030:	ff 25 e2 2f 00 00    	jmpq   *0x2fe2(%rip)        # 4018 <__stack_chk_fail@GLIBC_2.4>
    1036:	68 00 00 00 00       	pushq  $0x0
    103b:	e9 e0 ff ff ff       	jmpq   1020 <.plt>

0000000000001040 <printf@plt>:
    1040:	ff 25 da 2f 00 00    	jmpq   *0x2fda(%rip)        # 4020 <printf@GLIBC_2.2.5>
    1046:	68 01 00 00 00       	pushq  $0x1
    104b:	e9 d0 ff ff ff       	jmpq   1020 <.plt>

0000000000001050 <__isoc99_sscanf@plt>:
    1050:	ff 25 d2 2f 00 00    	jmpq   *0x2fd2(%rip)        # 4028 <__isoc99_sscanf@GLIBC_2.7>
    1056:	68 02 00 00 00       	pushq  $0x2
    105b:	e9 c0 ff ff ff       	jmpq   1020 <.plt>

Disassembly of section .text:

0000000000001060 <_start>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	31 ed                	xor    %ebp,%ebp
    1066:	49 89 d1             	mov    %rdx,%r9
    1069:	5e                   	pop    %rsi
    106a:	48 89 e2             	mov    %rsp,%rdx
    106d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1071:	50                   	push   %rax
    1072:	54                   	push   %rsp
    1073:	4c 8d 05 76 02 00 00 	lea    0x276(%rip),%r8        # 12f0 <__libc_csu_fini>
    107a:	48 8d 0d ff 01 00 00 	lea    0x1ff(%rip),%rcx        # 1280 <__libc_csu_init>
    1081:	48 8d 3d a0 01 00 00 	lea    0x1a0(%rip),%rdi        # 1228 <main>
    1088:	ff 15 52 2f 00 00    	callq  *0x2f52(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
    108e:	f4                   	hlt    
    108f:	90                   	nop

0000000000001090 <deregister_tm_clones>:
    1090:	48 8d 3d a9 2f 00 00 	lea    0x2fa9(%rip),%rdi        # 4040 <__TMC_END__>
    1097:	48 8d 05 a2 2f 00 00 	lea    0x2fa2(%rip),%rax        # 4040 <__TMC_END__>
    109e:	48 39 f8             	cmp    %rdi,%rax
    10a1:	74 15                	je     10b8 <deregister_tm_clones+0x28>
    10a3:	48 8b 05 2e 2f 00 00 	mov    0x2f2e(%rip),%rax        # 3fd8 <_ITM_deregisterTMCloneTable>
    10aa:	48 85 c0             	test   %rax,%rax
    10ad:	74 09                	je     10b8 <deregister_tm_clones+0x28>
    10af:	ff e0                	jmpq   *%rax
    10b1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10b8:	c3                   	retq   
    10b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010c0 <register_tm_clones>:
    10c0:	48 8d 3d 79 2f 00 00 	lea    0x2f79(%rip),%rdi        # 4040 <__TMC_END__>
    10c7:	48 8d 35 72 2f 00 00 	lea    0x2f72(%rip),%rsi        # 4040 <__TMC_END__>
    10ce:	48 29 fe             	sub    %rdi,%rsi
    10d1:	48 c1 fe 03          	sar    $0x3,%rsi
    10d5:	48 89 f0             	mov    %rsi,%rax
    10d8:	48 c1 e8 3f          	shr    $0x3f,%rax
    10dc:	48 01 c6             	add    %rax,%rsi
    10df:	48 d1 fe             	sar    %rsi
    10e2:	74 14                	je     10f8 <register_tm_clones+0x38>
    10e4:	48 8b 05 05 2f 00 00 	mov    0x2f05(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable>
    10eb:	48 85 c0             	test   %rax,%rax
    10ee:	74 08                	je     10f8 <register_tm_clones+0x38>
    10f0:	ff e0                	jmpq   *%rax
    10f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10f8:	c3                   	retq   
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <__do_global_dtors_aux>:
    1100:	f3 0f 1e fa          	endbr64 
    1104:	80 3d 35 2f 00 00 00 	cmpb   $0x0,0x2f35(%rip)        # 4040 <__TMC_END__>
    110b:	75 33                	jne    1140 <__do_global_dtors_aux+0x40>
    110d:	55                   	push   %rbp
    110e:	48 83 3d e2 2e 00 00 	cmpq   $0x0,0x2ee2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1115:	00 
    1116:	48 89 e5             	mov    %rsp,%rbp
    1119:	74 0d                	je     1128 <__do_global_dtors_aux+0x28>
    111b:	48 8b 3d 16 2f 00 00 	mov    0x2f16(%rip),%rdi        # 4038 <__dso_handle>
    1122:	ff 15 d0 2e 00 00    	callq  *0x2ed0(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1128:	e8 63 ff ff ff       	callq  1090 <deregister_tm_clones>
    112d:	c6 05 0c 2f 00 00 01 	movb   $0x1,0x2f0c(%rip)        # 4040 <__TMC_END__>
    1134:	5d                   	pop    %rbp
    1135:	c3                   	retq   
    1136:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    113d:	00 00 00 
    1140:	c3                   	retq   
    1141:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    1148:	00 00 00 00 
    114c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001150 <frame_dummy>:
    1150:	f3 0f 1e fa          	endbr64 
    1154:	e9 67 ff ff ff       	jmpq   10c0 <register_tm_clones>

0000000000001159 <explode_bomb>:
    1159:	55                   	push   %rbp
    115a:	48 89 e5             	mov    %rsp,%rbp
    115d:	48 8d 3d a0 0e 00 00 	lea    0xea0(%rip),%rdi        # 2004 <_IO_stdin_used+0x4>
    1164:	b8 00 00 00 00       	mov    $0x0,%eax
    1169:	e8 d2 fe ff ff       	callq  1040 <printf@plt>
    116e:	48 8d 3d 96 0e 00 00 	lea    0xe96(%rip),%rdi        # 200b <_IO_stdin_used+0xb>
    1175:	b8 00 00 00 00       	mov    $0x0,%eax
    117a:	e8 c1 fe ff ff       	callq  1040 <printf@plt>
    117f:	90                   	nop
    1180:	5d                   	pop    %rbp
    1181:	c3                   	retq   

0000000000001182 <phase_1>:
    1182:	55                   	push   %rbp
    1183:	48 89 e5             	mov    %rsp,%rbp
    1186:	48 83 ec 30          	sub    $0x30,%rsp
    118a:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    118e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1195:	00 00 
    1197:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    119b:	31 c0                	xor    %eax,%eax
    119d:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
    11a4:	48 8d 7d f0          	lea    -0x10(%rbp),%rdi
    11a8:	48 8d 75 ec          	lea    -0x14(%rbp),%rsi
    11ac:	48 8d 4d e8          	lea    -0x18(%rbp),%rcx
    11b0:	48 8d 55 e4          	lea    -0x1c(%rbp),%rdx
    11b4:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    11b8:	49 89 f9             	mov    %rdi,%r9
    11bb:	49 89 f0             	mov    %rsi,%r8
    11be:	48 8d 35 59 0e 00 00 	lea    0xe59(%rip),%rsi        # 201e <_IO_stdin_used+0x1e>
    11c5:	48 89 c7             	mov    %rax,%rdi
    11c8:	b8 00 00 00 00       	mov    $0x0,%eax
    11cd:	e8 7e fe ff ff       	callq  1050 <__isoc99_sscanf@plt>
    11d2:	eb 04                	jmp    11d8 <phase_1+0x56>
    11d4:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)
    11d8:	8b 55 f0             	mov    -0x10(%rbp),%edx
    11db:	8b 45 f4             	mov    -0xc(%rbp),%eax
    11de:	8d 0c 02             	lea    (%rdx,%rax,1),%ecx
    11e1:	8b 55 e4             	mov    -0x1c(%rbp),%edx
    11e4:	8b 45 e8             	mov    -0x18(%rbp),%eax
    11e7:	01 d0                	add    %edx,%eax
    11e9:	39 c1                	cmp    %eax,%ecx
    11eb:	7c e7                	jl     11d4 <phase_1+0x52>
    11ed:	8b 45 ec             	mov    -0x14(%rbp),%eax
    11f0:	39 45 f4             	cmp    %eax,-0xc(%rbp)
    11f3:	74 0c                	je     1201 <phase_1+0x7f>
    11f5:	b8 00 00 00 00       	mov    $0x0,%eax
    11fa:	e8 5a ff ff ff       	callq  1159 <explode_bomb>
    11ff:	eb 11                	jmp    1212 <phase_1+0x90>
    1201:	48 8d 3d 22 0e 00 00 	lea    0xe22(%rip),%rdi        # 202a <_IO_stdin_used+0x2a>
    1208:	b8 00 00 00 00       	mov    $0x0,%eax
    120d:	e8 2e fe ff ff       	callq  1040 <printf@plt>
    1212:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1216:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    121d:	00 00 
    121f:	74 05                	je     1226 <phase_1+0xa4>
    1221:	e8 0a fe ff ff       	callq  1030 <__stack_chk_fail@plt>
    1226:	c9                   	leaveq 
    1227:	c3                   	retq   

0000000000001228 <main>:
    1228:	55                   	push   %rbp
    1229:	48 89 e5             	mov    %rsp,%rbp
    122c:	48 83 ec 10          	sub    $0x10,%rsp
    1230:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1237:	00 00 
    1239:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    123d:	31 c0                	xor    %eax,%eax
    123f:	48 b8 32 20 36 20 35 	movabs $0x33203520362032,%rax
    1246:	20 33 00 
    1249:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    124d:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
    1251:	48 89 c7             	mov    %rax,%rdi
    1254:	e8 29 ff ff ff       	callq  1182 <phase_1>
    1259:	b8 00 00 00 00       	mov    $0x0,%eax
    125e:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    1262:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
    1269:	00 00 
    126b:	74 05                	je     1272 <main+0x4a>
    126d:	e8 be fd ff ff       	callq  1030 <__stack_chk_fail@plt>
    1272:	c9                   	leaveq 
    1273:	c3                   	retq   
    1274:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    127b:	00 00 00 
    127e:	66 90                	xchg   %ax,%ax

0000000000001280 <__libc_csu_init>:
    1280:	f3 0f 1e fa          	endbr64 
    1284:	41 57                	push   %r15
    1286:	49 89 d7             	mov    %rdx,%r15
    1289:	41 56                	push   %r14
    128b:	49 89 f6             	mov    %rsi,%r14
    128e:	41 55                	push   %r13
    1290:	41 89 fd             	mov    %edi,%r13d
    1293:	41 54                	push   %r12
    1295:	4c 8d 25 4c 2b 00 00 	lea    0x2b4c(%rip),%r12        # 3de8 <__frame_dummy_init_array_entry>
    129c:	55                   	push   %rbp
    129d:	48 8d 2d 4c 2b 00 00 	lea    0x2b4c(%rip),%rbp        # 3df0 <__init_array_end>
    12a4:	53                   	push   %rbx
    12a5:	4c 29 e5             	sub    %r12,%rbp
    12a8:	48 83 ec 08          	sub    $0x8,%rsp
    12ac:	67 e8 4e fd ff ff    	addr32 callq 1000 <_init>
    12b2:	48 c1 fd 03          	sar    $0x3,%rbp
    12b6:	74 1e                	je     12d6 <__libc_csu_init+0x56>
    12b8:	31 db                	xor    %ebx,%ebx
    12ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    12c0:	4c 89 fa             	mov    %r15,%rdx
    12c3:	4c 89 f6             	mov    %r14,%rsi
    12c6:	44 89 ef             	mov    %r13d,%edi
    12c9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
    12cd:	48 83 c3 01          	add    $0x1,%rbx
    12d1:	48 39 dd             	cmp    %rbx,%rbp
    12d4:	75 ea                	jne    12c0 <__libc_csu_init+0x40>
    12d6:	48 83 c4 08          	add    $0x8,%rsp
    12da:	5b                   	pop    %rbx
    12db:	5d                   	pop    %rbp
    12dc:	41 5c                	pop    %r12
    12de:	41 5d                	pop    %r13
    12e0:	41 5e                	pop    %r14
    12e2:	41 5f                	pop    %r15
    12e4:	c3                   	retq   
    12e5:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    12ec:	00 00 00 00 

00000000000012f0 <__libc_csu_fini>:
    12f0:	f3 0f 1e fa          	endbr64 
    12f4:	c3                   	retq   

Disassembly of section .fini:

00000000000012f8 <_fini>:
    12f8:	f3 0f 1e fa          	endbr64 
    12fc:	48 83 ec 08          	sub    $0x8,%rsp
    1300:	48 83 c4 08          	add    $0x8,%rsp
    1304:	c3                   	retq   
