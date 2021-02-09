#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

int RowA,ColA,ColB, **m, **m1,**m2;
int NUM_COL,NUM_ROW;
int n;

void * remplir (){
    // remplir la matrice 1
    printf("le nombre de ligne de A !\n");
    scanf("%d",&RowA);
    printf("le nombre de colonne de A  !\n");
    scanf("%d",&ColA);
    int i,j ;
    m=(int**) malloc (RowA*sizeof(int*)) ;
    for (i=0 ; i<RowA ;i++)
        m[i]=  malloc (ColA*sizeof(int)) ;
    printf("matrice 1:\n");
    for (i=0 ; i<RowA ;i++)
    { for (j=0 ; j<ColA ;j++)
        {m[i][j] = rand()%10;
            printf("%d  ",m[i][j]);
        }
        printf("\n");
    }
    //remplir la matrice 2

    printf("le nombre de Colonne de B !\n");
    scanf("%d",&ColB);
    m1=(int**) malloc (ColA*sizeof(int*)) ;
    for (i=0 ; i<ColA ;i++)
        m1[i]=  malloc (ColB*sizeof(int)) ;
    printf("matrice 2:\n");
    for (i=0 ; i<ColA;i++)
    { for (j=0 ; j<ColB;j++)
        {m1[i][j] = rand()%10;
            printf("%d  ",m1[i][j]);
        }
        printf("\n");
    }
    printf("Nombre de bloque en  ligne:\n");
    scanf("%d",&NUM_ROW) ;
    printf("Nombre de bloque en  colonne:\n");
    scanf("%d",&NUM_COL);
    printf("Nombre de bloque en ligne et   colonne:\n");
    scanf("%d",&n);


}
// procedure multiplication avec K le numero du thread
void *  multiplication2Dim(void * th) {
    int i, j;
    int sum;

    long k = (long) th ;
    int remainder = k % NUM_COL;
    int quotient = k / NUM_COL;

    int debut,fin;
    debut=fin=0;
    int debut2, fin2;
    debut2 = fin2 = 0;
    debut=ColB/NUM_COL;
    debut2 =RowA/NUM_ROW;



    printf("thread numero %d\n", k);
    m2 = (int **) malloc(RowA* sizeof(int *));
    for (i = 0; i < RowA; i++)
        m2[i] = malloc(ColB* sizeof(int));
    if(remainder== NUM_COL-1){
        fin= ColB-1;

    }
    else{
        fin=NUM_COL/sqrtf(NUM_COL*NUM_ROW);

    }

    if(quotient == NUM_ROW-1){

        fin2 =  RowA - 1;
    }
    else{

        fin2 = NUM_ROW/sqrtf(NUM_ROW*NUM_COL);

    }




    for (i = debut2; i <=fin2; i++) {
        sum=0;
        for (j = debut; j <= fin; j++) {
            sum = sum + m[i][k] * m1[k][j];
            m2[i][j] = sum;
        }
        printf("\n");
    }

    pthread_exit(NULL);



}
void *  multiplication1Dim(void * th) {
    int i, j;
    int sum;

    long k = (long) th ;
    int remainder = k %n;
    int quotient = k / n;

    int debut,fin;
    debut=fin=0;
    int debut2, fin2;
    debut2 = fin2 = 0;
    debut=ColB/n;
    debut2 =RowA/n;



    printf("thread numero %d\n", k);
    m2 = (int **) malloc(RowA* sizeof(int *));
    for (i = 0; i < RowA; i++)
        m2[i] = malloc(ColB* sizeof(int));
    if(remainder== n-1){
        fin= ColB-1;

    }
    else{
        fin=debut+(ColB/n)-1;

    }

    if(quotient ==n-1){

        fin2 =  RowA - 1;
    }
    else{

        fin2 = debut2 + (RowA/n)-1;

    }




    for (i = debut2; i <=fin2; i++) {
        sum=0;
        for (j = debut; j <= fin; j++) {
            sum = sum + m[i][k] * m1[k][j];
            m2[i][j] = sum;
        }
        printf("\n");
    }

    pthread_exit(NULL);



}

int main() {
    pthread_t threads[NUM_ROW*NUM_COL];
    int rc;
    int th;
    int sum;
    double debut,fin;
    char rep;
    srand(time(NULL));
    remplir();
    printf("--------------------le calcul du produit matricielle parallele 1 Dimension--------------------------\n");
    debut = clock();


    for(th = 0; th < NUM_COL * NUM_ROW ; th++){

        rc = pthread_create(&threads[th], NULL, multiplication1Dim,(void *) th);

        if (rc){
            printf("Erreur de creation de thread; code erreur = %d\n", rc);
            exit(-1);
        }
    } for(th = 0; th < NUM_COL * NUM_ROW; th++){
        (void)pthread_join(threads[th],NULL);
    }

    fin =  clock();
    printf("--------------------le calcul du produit matricielle parallele 2 Dimension--------------------------\n");
    debut = clock();


    for(th = 0; th < NUM_COL * NUM_ROW ; th++){

        rc = pthread_create(&threads[th], NULL, multiplication2Dim,(void *) th);

        if (rc){
            printf("Erreur de creation de thread; code erreur = %d\n", rc);
            exit(-1);
        }
    } for(th = 0; th < NUM_COL * NUM_ROW; th++){
        (void)pthread_join(threads[th],NULL);
    }

    fin =  clock();


    printf("------------matrice resultat -----------\n");

    for(int i = 0 ; i < ColA ; i++){
        for (int j = 0 ; j < ColA ; j++)
            printf("%d ",m2[i][j]);
        printf(" \n");
    }
    printf("temps parralele =%f\n", (double)(fin-debut)/CLOCKS_PER_SEC);



    printf("--------------------le calcul du produit matricielle sequentielle-----------------------\n");
    debut =  clock();
    for(int i=0; i<RowA; i++){
        for(int j=0; j<ColB; j++){
            sum=0;
            for(int k=0; k<ColA; k++)
                sum = sum + m[i][k] * m1[k][j];
            m2[i][j] = sum;

        }
    }
    fin =  clock();

    printf("------------matrice resultat -----------\n");

    for(int i = 0 ; i <RowA ; i++){
        for (int j = 0 ; j < ColB ; j++)
            printf("%d ",m2[i][j]);
        printf(" \n");
    }
    printf("temps sequentielle=%f\n", (double)(fin-debut)/CLOCKS_PER_SEC);
    free(m);
    free(m1);
    free(m2);
    return 0;
}



