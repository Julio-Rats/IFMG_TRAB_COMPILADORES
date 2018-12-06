#include "virtualmachine.h"

unsigned int lenVariables  = 0;
t_variable  *listVariables = NULL;


char* genLabel(){
      static unsigned short int inc = 0;
      char   *aux   = malloc(sizeof(char)*(strlen("_label")+6));
      sprintf(aux,"%s%d","_l",inc++);
      return aux;
}
char* genTemp(){
      static unsigned short int inc = 0;
      char   *aux   = malloc(sizeof(char)*(strlen("_temp")+6));
      sprintf(aux,"%s%d","_t",inc++);
      return aux;
}
Quad* genQuad(char*param1,char*param2,char*param3,char*param4){
      Quad *aux   = (Quad*) malloc(sizeof(Quad));
      aux->param1 = param1;
      aux->param2 = param2;
      aux->param3 = param3;
      aux->param4 = param4;
      aux->next   = NULL;
      return aux;
}
Quad* addQuad(Quad* destine, Quad* source){
      Quad *aux = destine;
      if (aux){
          while(aux->next){
              aux = aux->next;
              // printf("%p\n", aux);
          }
          if (source){
              aux->next = malloc(sizeof(Quad*));
              aux->next = source;
          }
          return destine;
      }
      return source;
}

Quad* copyQuad(Quad* list){
    Quad* aux3, *aux2, *aux = malloc(sizeof(Quad));
    aux2 = list;
    aux3 = aux;
    while(aux2){
        aux3->param1 = malloc(sizeof(char)*5);
        strcpy(aux3->param1, aux2->param1);
        if (aux2->param2){
          aux3->param2 = malloc(sizeof(char)*34);
          strcpy(aux3->param2, aux2->param2);
        }else{
          aux3->param2 = NULL;
        }
        if (aux2->param3){
          aux3->param3 = malloc(sizeof(char)*34);
          strcpy(aux3->param3, aux2->param3);
        }else{
          aux3->param3 = NULL;
        }
        if (aux2->param4){
          aux3->param4 = malloc(sizeof(char)*34);
          strcpy(aux3->param4, aux2->param4);
        }else{
          aux3->param4 = NULL;
        }
        aux3->next = NULL;
        aux2 = aux2->next;
        if (aux2){
          aux3->next = malloc(sizeof(Quad));
          aux3 = aux3->next;
        }
    }
    return aux;
}

void exec(Quad *lista){
    Quad *aux = lista;
    while(aux){
        int    opaux, type;
        int    op = decod_inst(aux->param1);
        float valor;
        char   str[64];
        switch (op) {
          case 0:
              type  = getType(aux->param2);
              valor = getValue(aux->param3);
              add_var(aux->param2, valor, type);
          break;

          case 1:
              valor = getValue(aux->param3) == getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 2:
              valor = getValue(aux->param3) > getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 3:
              valor = getValue(aux->param3) >= getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 4:
              valor = getValue(aux->param3) < getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 5:
              valor = getValue(aux->param3) <= getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 6:
              valor = getValue(aux->param3) != getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 7:
              valor = !getValue(aux->param3);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 8:
              valor = getValue(aux->param3) && getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 9:
              valor = getValue(aux->param3) || getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 10:
              if (getValue(aux->param2)){
                  aux = getLabel(lista, aux->param3);
              }else{
                  aux = getLabel(lista, aux->param4);
              }
          break;

          case 11:
              aux = getLabel(lista, aux->param2);
          break;

          case 12:
              opaux = decod_inst(aux->param2);
              if (opaux == 13){
                   sprintf(str, aux->param3);
                   if (str[0] == '\"'){
                      strcpy(str, removeaspas(str));
                      strcpy(str, interpretaStr(str));
                      printf("%s", str);
                   }else if (str[0] == '_'){
                      printf("%.2f\n", getValue(str));
                   }else{
                      printf("%.2f\n", atof(str));
                   }
              }else{
                  sprintf(str, aux->param3);
                  strcpy(str, removeaspas(str));
                  strcpy(str, interpretaStr(str));
                  printf("%s", str);
                  scanf("%f", &valor);
                  type  = getType(aux->param4);
                  add_var(aux->param4, valor, type);
              }
          break;

          case 15:
              valor = getValue(aux->param3)+getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;

          case 16:
              valor = getValue(aux->param3)-getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;
          case 17:
              valor = getValue(aux->param3)*getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;
          case 18:
              valor = getValue(aux->param3)/getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;
          case 19:
              valor = (int)getValue(aux->param3) % (int)getValue(aux->param4);
              type  = getType(aux->param2);
              add_var(aux->param2, valor, type);
          break;
        }
        aux = aux->next;
    }
}

int decod_inst(char *opcode){
    if (strcmp(opcode, "=")==0){
          return 0;
    }else if (strcmp(opcode, "==")==0){
          return 1;
    }else if (strcmp(opcode, ">")==0){
          return 2;
    }else if (strcmp(opcode, ">=")==0){
          return 3;
    }else if (strcmp(opcode, "<")==0){
          return 4;
    }else if (strcmp(opcode, "<=")==0){
          return 5;
    }else if (strcmp(opcode, "!=")==0){
          return 6;
    }else if (strcmp(opcode, "!")==0){
          return 7;
    }else if (strcmp(opcode, "&&")==0){
          return 8;
    }else if (strcmp(opcode, "||")==0){
          return 9;
    }else if (strcmp(opcode, "IF")==0){
          return 10;
    }else if (strcmp(opcode, "JUMP")==0){
          return 11;
    }else if (strcmp(opcode, "CALL")==0){
          return 12;
    }else if (strcmp(opcode, "PRINT")==0){
          return 13;
    }else if (strcmp(opcode, "SCAN")==0){
          return 14;
    }else if (strcmp(opcode, "+")==0){
          return 15;
    }else if (strcmp(opcode, "-")==0){
          return 16;
    }else if (strcmp(opcode, "*")==0){
          return 17;
    }else if (strcmp(opcode, "/")==0){
          return 18;
    }else if (strcmp(opcode, "%")==0){
          return 19;
    }
    return -1;
}

void add_var(char *id, float value,  int type){
    if (lenVarambiente == 0){
        listVarambiente = malloc(sizeof(t_varambiente));
        lenVarambiente  = 1;
        listVarambiente[0].id_var = id;
        if (type == 0){
            listVarambiente[0].value_numeric = trunc(value);
            listVarambiente[0].type = 0;
        }else if (type == 1){
            listVarambiente[0].value_numeric = value;
              listVarambiente[0].type = 1;
        }
        return;
    }
    for(int i=0;i<lenVarambiente;i++)
        if (strcmp(listVarambiente[i].id_var, id)==0){
          if (type == 0){
              listVarambiente[i].value_numeric = trunc(value);
              listVarambiente[i].type = 0;
          }else if (type == 1){
              listVarambiente[i].value_numeric = value;
              listVarambiente[i].type = 1;
          }
          return;
        }

    listVarambiente = realloc(listVarambiente, sizeof(t_varambiente)*(++lenVarambiente));
    listVarambiente[(lenVarambiente)-1].id_var = id;
    listVarambiente[(lenVarambiente)-1].type   = type;

    if (type == 0){
        listVarambiente[(lenVarambiente)-1].value_numeric = trunc(value);
    }else if (type == 1){
        listVarambiente[(lenVarambiente)-1].value_numeric = value;
    }

}

int getType(char *lexema){
    char *aux = malloc (sizeof(char)*40);
    for(int i=0;i<lenVariables;i++){
        sprintf(aux,"_%d%s",listVariables[i].nivel, listVariables[i].id_var);
        if (strcmp(lexema, aux)==0){
          return listVariables[i].type;
        }
    }
    free(aux);
    return 1;
}

float getValue(char *lexema){
    for(int i=0;i<lenVarambiente;i++){
        if (strcmp(lexema, listVarambiente[i].id_var)==0){
            return listVarambiente[i].value_numeric;
        }
    }
    return atof(lexema);
}

Quad* getLabel(Quad* list, char* lexema){
    Quad *aux = list;
    while(aux){
      if (strcmp((char*)"LABEL", aux->param1)==0)
         if (strcmp(lexema, aux->param2)==0)
            return aux;
      aux = aux->next;
    }
    return NULL;
}

char* removeaspas(char* str){
      int len = strlen(str);
      char* aux = malloc(sizeof(char)*(len+1));
      for(int i=0;i<len-2;i++){
          aux[i] = str[i+1];
      }
      return aux;
}

char* interpretaStr(char* str){
      int len   = strlen(str);
      char* aux = malloc(sizeof(char)*len);
      unsigned int indice = 0;
      for(int i=0;i<len;i++){
        if((str[i]=='\\')&&(str[i-1])!='\\'){
            switch (str[i+1]) {
              case 'n':
                  aux[indice] = '\0';
                  sprintf(aux,"%s\n",aux);
                  indice +=2;
                  i += 2;
              break;
              case 't':
                  aux[indice] = '\0';
                  sprintf(aux,"%s\t",aux);
                  indice +=2;
                  i += 2;
              break;
              default:
                  // aux[i] = str[i+1];
                  continue;
              break;
            }
        }
        aux[indice++] = str[i];
      }
    aux[indice] = '\0';
    return aux;
}
