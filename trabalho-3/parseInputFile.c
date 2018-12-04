void parseProcessFile(char *filename, struct Queue *processCreation){
    FILE *file = fopen(filename, "r");
    char *buffer;
    size_t n = 0;
    int c;
    if(file == NULL){
        printf("ERRO AO ABRIRARQUIVO");
        exit(1);
    }
    buffer = (char *)malloc(20);
    while((c = fgetc(file)) != EOF){
        if(c == ';'){
            buffer[n] = '\0';
            // printf("%s\n", buffer);
            enQueue(processCreation, atoi(buffer));
            n = 0;
            continue;
        }
        // else if(c == ';'){
        //     buffer[n] = '\0';
        //     // printf("%s\n", buffer);
        //     newProc->priority = atoi(buffer);
        //     n = 0;
        //     continue;
        // }
        buffer[n++] = (char) c;
    }
    free(buffer);
    fclose(file);
}

