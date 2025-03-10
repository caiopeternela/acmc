#include "globals.h"
#include "util.h"

// Imprime o token e seu lexema no arquivo de listagem
void printToken(TokenType token, const char *tokenString) {
  switch (token) {
    case IF:
    case ELSE:
    case RETURN:
    case WHILE:
    case INT:
    case VOID:
      fprintf(listing, "reserved word: %s\n", tokenString);
      break;
    case MAIS: fprintf(listing, "+\n"); break;
    case SUB: fprintf(listing, "-\n"); break;
    case MULT: fprintf(listing, "*\n"); break;
    case DIV: fprintf(listing, "/\n"); break;
    case IGUAL: fprintf(listing, "=\n"); break;
    case IGDAD: fprintf(listing, "==\n"); break;
    case DIFER: fprintf(listing, "!=\n"); break;
    case MENOR: fprintf(listing, "<\n"); break;
    case MENIG: fprintf(listing, "<=\n"); break;
    case MAIOR: fprintf(listing, ">\n"); break;
    case MAIIG: fprintf(listing, ">=\n"); break;
    case APAR: fprintf(listing, "(\n"); break;
    case FPAR: fprintf(listing, ")\n"); break;
    case ACOL: fprintf(listing, "[\n"); break;
    case FCOL: fprintf(listing, "]\n"); break;
    case ACHAV: fprintf(listing, "{\n"); break;
    case FCHAV: fprintf(listing, "}\n"); break;
    case PV: fprintf(listing, ";\n"); break;
    case VIR: fprintf(listing, ",\n"); break;
    case ENDFILE: fprintf(listing, "EOF\n"); break;
    case NUM:
      fprintf(listing, "NUM, val= %s\n", tokenString);
      break;
    case ID:
      fprintf(listing, "ID, name= %s\n", tokenString);
      break;
    case ERROR:
      fprintf(listing, "ERROR: %s\n", tokenString);
      break;
    default:
      fprintf(listing, "Unknown token: %d\n", token);
  }
}

// Cria e inicializa um novo nó de declaração para a árvore sintática
TreeNode *newStmtNode(StmtKind kind) {
  TreeNode *t = malloc(sizeof(TreeNode));
  int i;
  if (t == NULL) {
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  } else {
    for (i = 0; i < MAXCHILDREN; i++) {
      t->child[i] = NULL;
    }
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

// Cria e inicializa um novo nó de expressão para a árvore sintática
TreeNode *newExpNode(ExpKind kind) {
  TreeNode *t = malloc(sizeof(TreeNode));
  int i;
  if (t == NULL) {
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  } else {
    for (i = 0; i < MAXCHILDREN; i++) {
      t->child[i] = NULL;
    }
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

// Cria uma cópia de uma string, alocando a memória necessária para ela
char *copyString(char *s) {
  int n;
  char *t;
  if (s == NULL) {
    return NULL;
  }
  n = strlen(s) + 1;
  t = malloc(n);
  if (t == NULL) {
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  } else {
    strcpy(t, s);
  }
  return t;
}

static int indentno = 0;  // Controla a quantidade de espaços para indentação na impressão da árvore

// Macros para aumentar e diminuir a indentação
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

// Imprime os espaços correspondentes à indentação atual
static void printSpaces(void) {
  for (int i = 0; i < indentno; i++) {
    fprintf(listing, " ");
  }
}

// Imprime a árvore sintática no arquivo de listagem, utilizando indentação para mostrar a hierarquia
void print_tree(TreeNode *tree) {
  int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind == StmtK) {
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing, "If\n");
          break;
        case AssignK:
          fprintf(listing, "Assign: \n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case ReturnK:
          fprintf(listing, "Return\n");
          break;
        default:
          fprintf(listing, "Unknown StmtKNode kind\n");
          break;
      }
    } else if (tree->nodekind == ExpK) {
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing, "Oper: ");
          printToken(tree->attr.opr, "\0");
          break;
        case ConstK:
          fprintf(listing, "Const: %d\n", tree->attr.val);
          break;
        case IdK:
          fprintf(listing, "Id: %s\n", tree->attr.name);
          break;
        case FuncK:
          fprintf(listing, "Func: %s\n", tree->attr.name);
          break;
        case VarK:
          fprintf(listing, "Var: %s\n", tree->attr.name);
          break;
        case CallK:
          fprintf(listing, "Call func: %s\n", tree->attr.name);
          break;
        case TypeK:
          fprintf(listing, "Type: %s\n", tree->attr.name);
          break;
        case ParamK:
          fprintf(listing, "Param: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing, "Unknown ExpKNode kind\n");
          break;
      }
    } else {
      fprintf(listing, "Unknown node kind\n");
    }
    for (i = 0; i < MAXCHILDREN; i++) {
      print_tree(tree->child[i]);
    }
    tree = tree->sibling;
  }
  UNINDENT;
}
