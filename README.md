<h1 align="center">Welcome to assembly-lang-translator 👋</h1>
<p>
</p>

## Author

👤 **Orydoroha Mykhailo**

* Github: [Mykhailo Orydoroha](https://github.com/orydoroha-mykhailo)
* LinkedIn: [Mykhailo Orydoroha](https://linkedin.com/in/mykhailo-o-948b03133)
# Listing generated by this translator
```asm
Assembly Translator.
Written by Orydoroha Mykhailo.

Filename: test.asm



     1	0000                    DATA SEGMENT 
     2	0000                    STRING DB "LOREM" 
     3	0005                    VAR0BD DB 123D 
     4	0006                    VAR0WH DW 0AB12H 
     5	0008                    VAR0DH DD 06E6CCH 
     6	000C                    DATA ENDS 
     7	0000                    CODE1 SEGMENT 
     8	0000                    PROC1 PROC FAR 
     9	0000                    ;  IMUL TESTS 
    10	0000                    IMUL AX , WORD PTR [ EDX + ESI * 4 + 6 ] , 10 
    11	0006                    IMUL EAX , DWORD PTR [ EDX + ESI * 4 + 6 ] , 10 
    12	000D                    IMUL AX , WORD PTR [ EDX + ESI * 4 + 6 ] , 1000 
    13	0014                    IMUL EAX , DWORD PTR [ EDX + ESI * 4 + 6 ] , 33000 
    14	001E                    ;  PUSH TESTS 
    15	001E                    PUSH DWORD PTR [ EDX + ESI * 4 + 6 ] 
    16	0024                    PUSH WORD PTR [ EDX + ESI * 4 + 6 ] 
    17	0029                    ;  MOV TESTS 
    18	0029                    MOV BYTE PTR [ EDX + ESI * 4 + 6 ] , 10 
    19	002F                    MOV EAX , [ EDX + ESI * 4 + 6 ] 
    20	0035                    POP EAX 
    21	0037                    RET 
    22	0038                    PROC1 ENDP 
    23	0038                    ;  
    24	0038                    ;  OR TESTS  
    25	0038                    OR AX , BX 
    26	003A                    OR EAX , EBX 
    27	003D                    OR AL , BL 
    28	003F                    OR BX , AX ;  VALID 
    29	0041                    MOV SI , [ EDX + ESI * 4 + 6 ] 
    30	0046                    JZ VOL 
    31	004A                    VOL : 
    32	004A                    CODE1 ENDS 
    33	0000                    CODE2 SEGMENT 
    34	0000                    PROC2 PROC 
    35	0000                    RET 
    36	0001                    PROC2 ENDP 
    37	0001                    OR CL , AH 
    38	0003                    CALL PROC2 
    39	0006                    CALL PROC1 
    40	000B                    CODE2 ENDS 
    41	000B                    END 
```

## Show your support
Give a ⭐️ if this project helped you!
