/**
* @file shell.c
* @brief Implementacion de un shell para Pintos
* @author Ivan Valette
*/

#include "lib/stdio.h"
#include "kernel/console.h"
#include "lib/stdlib.h"
#include "devices/input.h"
#include "lib/string.h"


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "filesys/file.h"
#include "filesys/filesys.h"

#define INPUT_SIZE 50
#define CARRIAGE_RETURN_CHAR 13
#define MATCH_SUCCESS 0
#define FILE_SIZE 40960

char username[] = "Ivan Valette";


void remove_file_verbose(char *);
void remove_file(char *);
bool rename_file(char *, char *);
/**
 *Shell para Pintos capaz de reconocer los comandos
 *whoami: imprime el nombre del usuario en consola
 *exit: concluye el shell y permite que Pintos termine
 *
 *En caso de que se teclee un comando diferente a los descritos anteriormente
 *se informara al usuario que el comando es desconocido.
 */

int shell()
{
     int running = 1;
     
    
    while(running == 1)
    {
        char input[INPUT_SIZE];
        char *user_input;
        char *context = NULL;
            printf("Shell ISC364>");

            for(int i = 0; i < INPUT_SIZE; i++) {
        	    input[i] = input_getc();
                
                if(input[i] == CARRIAGE_RETURN_CHAR)
                {
                    input[i] = '\0';
                    break;
                }
                    
                 printf("%c", input[i]);
            }

            user_input = strtok_r(input, " ", &context);

            /*Si se entra la cadena "whoami"*/
            if(strcmp(user_input, "whoami") == MATCH_SUCCESS) 
            { 
                printf("\n%s\n", username);
                
            }
            /*Si se entra la cadena "exit"*/
            else if(strcmp(user_input, "exit") == MATCH_SUCCESS) 
            {
            	printf("\n");
                running = 0;
                break;
            }
            /*Crea un archivo, con proposito de debuggeo.*/
            else if(strcmp(user_input, "create") == MATCH_SUCCESS)
            {
                char **argv;
                user_input = strtok_r(NULL, " ", &context);
                filesys_create(user_input, FILE_SIZE);

            }
            /*Lista los archivos presentes en el disco, con proposito de debuggeo.*/
            else if(strcmp(user_input, "list-files") == MATCH_SUCCESS)
            {
                char **argv;
                printf("\n");
                fsutil_ls(argv);
            }
            /*Si se entra el comando rm*/
            else if(strcmp(user_input, "rm") == MATCH_SUCCESS)
            {
                user_input = strtok_r(NULL, " ", &context);
                /*Si el comando rm viene con el flag force*/
                if(strcmp(user_input, "-f") == MATCH_SUCCESS || strcmp(user_input, "--force") == MATCH_SUCCESS)
                {
                    user_input = strtok_r(NULL, " ", &context);
                    while(user_input != NULL)
                    {
                        filesys_remove(user_input);
                        user_input = strtok_r(NULL, " ", &context);
                    }

                }
                /*Si el comando rm viene con el flag help*/
                else if(strcmp(user_input, "--help") == MATCH_SUCCESS)
                {
                    print_rm_help_info();
                }
                /*Si el comando rm viene con el flag verbose*/
                else if(strcmp(user_input, "-v") == MATCH_SUCCESS || strcmp(user_input, "--verbose") == MATCH_SUCCESS)
                {
                    user_input = strtok_r(NULL, " ", &context);

                    while(user_input != NULL)
                    {
                        remove_file_verbose(user_input);
                        user_input = strtok_r(NULL, " ", &context);
                    }
                    
                    
                }
                /*Si el comando rm viene con el flag version*/
                else if(strcmp(user_input, "--version") == MATCH_SUCCESS)
                {
                    printf_rm_version_info();
                }
                else /*caso del comando rm sin ningun flag*/
                {
                    while(user_input != NULL)
                    {
                        remove_file(user_input);
                        user_input = strtok_r(NULL, " ", &context);
                    }
                }
                
            }
            /*Si se entra el comando mv*/
            else if(strcmp(user_input, "mv") == MATCH_SUCCESS)
            {
                user_input = strtok_r(NULL, " ", &context);

                /*Si el comando mv viene con el flag T*/
                if(strcmp(user_input, "-T") == MATCH_SUCCESS || strcmp(user_input, "no-match-directory") == MATCH_SUCCESS)
                {
                    char *original_file_name = strtok_r(NULL, " ", &context);
                    char *new_file_name = strtok_r(NULL, " ", &context);

                    if(filesys_open(new_file_name))
                    {
                        filesys_remove(new_file_name);
                    }

                    rename_file(original_file_name, new_file_name);

                }
                /*Si el comando mv viene con el flag n*/
                else if(strcmp(user_input, "-n") == MATCH_SUCCESS || strcmp(user_input, "no-clobber") == MATCH_SUCCESS)
                {
                    char *original_file_name = strtok_r(NULL, " ", &context);
                    char *new_file_name = strtok_r(NULL, " ", &context);

                    if(filesys_open(new_file_name))
                    {
                        printf("\nYa existe un archivo de nombre %s\n", new_file_name);
                    }
                    else
                    {
                        rename_file(original_file_name, new_file_name);
                    }
                }
                /*Si el comando mv viene con el flag help*/
                else if(strcmp(user_input, "--help") == MATCH_SUCCESS)
                {
                    printf_mv_help_info();
                }
                /*Si el comando mv viene con el flag version*/
                else if(strcmp(user_input, "--version") == MATCH_SUCCESS)
                {
                    print_mv_version_info();
                }
                /*SI el comando mv viene con el flag verbose*/
                else if(strcmp(user_input, "--verbose") == MATCH_SUCCESS || strcmp(user_input, "-v") == MATCH_SUCCESS)
                {
                    char *original_file_name = strtok_r(NULL, " ", &context);
                    char *new_file_name = strtok_r(NULL, " ", &context);

                    if(filesys_open(new_file_name))
                    {
                        filesys_remove(new_file_name);
                    }

                    if(rename_file(original_file_name, new_file_name))
                    {
                        printf("\n%s -> %s\n", original_file_name, new_file_name);
                    }
                    else
                    {
                        printf("\nHubo un error\n");
                    }
                    
                }
                else /*Caso del comando mv sin ningun flag*/
                {
                    char *original_file_name = user_input;
                    char *new_file_name = strtok_r(NULL, " ", &context);

                    if(filesys_open(new_file_name))
                    {
                        filesys_remove(new_file_name);
                    }

                    rename_file(original_file_name, new_file_name);
                }
            }
            /*Caso en que el comando no este definido*/
           else {
                
                printf("\nComando desconocido\n"); 
           } 
            
    }

    return 0;
}

/*Imprime informacion acerca de como usar el comando rm*/
void print_rm_help_info()
{
    printf("\n\nUso: rm [Opcion]. . . [ARCHIVO]. . . ");
    printf("\nBorra el ARCHIVO.");
    printf("\n-f, --force   Ignora la presencia de ficheros inexistentes. No escribe en consola..");
    printf("\n-v, --verbose   Escribe en consola una descripcion de lo realizado");
    printf("\n--version   Escribe en consola informacion de la version incluyendo el nombre del autor.\n\n");
}

/*Borra un archivo e informa del resultado de este proceso.*/
void remove_file_verbose(char *user_input)
{
    if(filesys_remove(user_input))
    {
        printf("\n%s fue borrado satisfactoriamente\n", user_input);
    }
    else
    {
        printf("\nHubo un problema borrando %s\n", user_input);
    }
}

/*Borra un archivo e informa si hubo un error haciendolo*/
void remove_file(char *user_input)
{
    if(!filesys_remove(user_input))
    {
        printf("\nHubo un problema borrando %s\n", user_input);
    }
}

/*Imprime informacion de la version actual*/

void printf_rm_version_info()
{
    printf("\nrm (Pintos) 1.0\n");
    printf("\nEscrito por %s\n", username);
}

/*Renombra un archivo y retorna true si el proceso fue exitoso, retorna false de lo contrario*/
bool rename_file(char * original_file_name, char * new_file_name)
{
    if(original_file_name != NULL && new_file_name != NULL)
    {
        struct file *file = filesys_open(original_file_name);
        if(file == NULL)
        {
            return false;
        }
        off_t file_size = file_length(file);
        filesys_create(new_file_name, file_size);

        void *buffer = malloc(file_size);

        file_read(file, buffer, file_size);

        struct file *newfile = filesys_open(new_file_name);
        

        file_write(newfile, buffer, file_size);

        file_close(file);
        file_close(newfile);
        
        filesys_remove(original_file_name);
        free(buffer);

        return true;
    }
    else
    {
        return false;
    }
    
}

void printf_mv_help_info()
{
    printf("\n\nUso: mv [OPCION] [ARCHIVO1] [ARCHIVO2]");
    printf("\nCambia el nombre de un archivo.");
    printf("\n-T, --no-target-directory   Cambia el nombre de un fichero por otro.");
    printf("\n-n, --no-cluster   Cambia el nombre de un archivo mientras no se este sobre escribiendo otro en el proceso.");
    printf("\n-v, --verbose   Escribe en consola una descripcion de lo realizado");
    printf("\n--version   Escribe en consola informacion de la version incluyendo el nombre del autor.\n\n");
}

void print_mv_version_info()
{
    printf("\nmv (Pintos) 1.0\n");
    printf("\nEscrito por %s\n", username);
}

