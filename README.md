# Analisador Léxico e Sintático C--

<!-- Os arquivos são os esqueletos disponibilizados pela Bianca eu não sabia qual dos princiapais escolher, então tem um de cada etapa: Etapa 1 Lexico, Etapa 2 Sintatico e Etapa 3 Semantico -->

## Analisador Léxico

A seguir, são apresentadas as definições regulares dos tokens de C--, nas quais os símbolos terminais estão destacados em **negrito**:
1. _letter_ → **[a-zA-Z]**
2. _digit_ → **[0-9]**
3. _ID_ → letter (**_letter_** | **_digit_** | **_** **)** **∗**
4. _integerconstant_ → **_digit_+**
5. _charconstant_ → ‘**ch**’ | ‘**\n**’ | ‘**\0**’, onde ch é _qualquer_ caractere ASCII _imprimível_ (como especificado pela função isprint(), **exceto \ e aspas simples**)
6. _stringconstant_ → “**(ch)***”, onde ch é _qualquer_ caractere ASCII _imprimível_ (como especificado pela função isprint(), **exceto aspas duplas e \n**)
7. _Operadores_ → **+** | **-** | **∗** | **/** | **=** | **==** | **!=** | **<=** | **<** | **>=** | **>** | **&&** | **||** | **!**
8. _Separadores_ → **(** | **)** | **{** | **}** | **[** | **]** | **,** | **;**
9. _Comentários_ devem ser **ignorados**. Seguem o formato de C e podem ser em linha ou em bloco. O comentário em linha é iniciado pelos caracteres **//** e encerrado por nova linha e o comentário em bloco é delimitado por **/*** e ***/**.
10. _Espaços em branco_ devem ser **ignorados**.

## Analisador Semântico

A gramática seguinte utiliza as notações ( N )∗ para representar 0 ou mais repetições de  N e a notação ( N )? para representar 0 ou 1 repetição de N. Os tokens da linguagem são  representados em vermelho e os não-terminais em itálico. 

1. Program → (Function)  ∗ EOF

2. Function → Type ID( ParamTypes ) { (Type VarDeclaration(, VarDeclaration) ∗ ; ) ∗ (Statement) ∗ } 
| void ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)  ∗ ; ) ∗ (Statement) ∗ }
 
3. VarDeclaration → ID ([integerconstant] )?

4. Type → char
| int

5. ParamTypes → void
| Type ID([])?(,Type ID([])?)∗

6. Statement → if ( Expression ) Statement (else Statement)?
| while ( Expression ) Statement
| for ( (Assign)?;(Expression)?;(Assign)? ) Statement
| return (Expression)?;
| Assign ;
| ID( (Expression( , Expression)  ∗)? ) ; 
| { (Statement) ∗ } 
| ;

7. Assign → ID ([ Expression ])? = Expression

8. Expression → - Expression
| ! Expression
| Expression BinOp Expression
| Expression Relop Expression
| Expression LogOp Expression
| ID(((Expression ( , Expression)∗)?)|[ Expression ])?
| ( Expression )
| integerconstant

| charconstant
| stringconstant

9. BinOp → +
| -
| *
| / 
 
10. RelOp → ==
| !=
| <=
| <
| >=
| > 
 
11. LogOp → &&
| ||

Possível mudança par LL1

Program → (Function)∗ EOF

Function → FuncType ID ( ParamTypes ) FunctionBody
FuncType → Type | void
FunctionBody → { (Type VarDeclaration (, VarDeclaration)∗ ; )∗ (Statement)∗ }

VarDeclaration → ID ArrayDims
ArrayDims → [ integerconstant ] | ε

Type → char | int

ParamTypes → VoidParams | TypedParams
VoidParams → void
TypedParams → Type ID ArrayDims MoreTypedParams
MoreTypedParams → , Type ID ArrayDims MoreTypedParams | ε

Statement → IfStatement
          | WhileStatement
          | ForStatement
          | ReturnStatement
          | AssignStatement
          | CallOrVarStatement
          | BlockStatement
          | EmptyStatement

IfStatement → if ( Expression ) Statement ElseClause
ElseClause → else Statement | ε
WhileStatement → while ( Expression ) Statement
ForStatement → for ( (Assign)? ; (Expression)? ; (Assign)? ) Statement
ReturnStatement → return (Expression)? ;
AssignStatement → Assign ;
CallOrVarStatement → ID ( (Expression (, Expression)∗)? ) ;
BlockStatement → { (Statement)∗ }
EmptyStatement → ;

Assign → ID ArrayAccess = Expression
ArrayAccess → [ Expression ] | ε

Expression → SimpleExpression Expression'
Expression' → BinOp SimpleExpression Expression' | ε
SimpleExpression → RelationalExpression SimpleExpression'
SimpleExpression' → LogOp RelationalExpression SimpleExpression' | ε
RelationalExpression → Term RelationalExpression'
RelationalExpression' → RelOp Term RelationalExpression' | ε
Term → Factor Term'
Term' → BinOp Factor Term' | ε
Factor → - Factor
       | ! Factor
       | ID ( (Expression (, Expression)∗)? )
       | ID [ Expression ]
       | ( Expression )
       | integerconstant
       | charconstant
       | stringconstant

BinOp → + | - | * | /
RelOp → == | != | <= | < | >= | >
LogOp → && | ||

EOF → end of file (definido implicitamente pelo analisador)