#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
int RowA,ColA,ColB, **m, **m1,**m2;

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
    for (i=0 ; i<ColB ;i++)
        m1[i]=  malloc (ColB*sizeof(int)) ;
    printf("matrice 2:\n");
    for (i=0 ; i<ColA;i++)
    { for (j=0 ; j<ColB;j++)
        {m1[i][j] = rand()%10;
            printf("%d  ",m1[i][j]);
        }
        printf("\n");
    }

}
// procedure multiplication avec K le numero du thread
void *  multiplication(int k) {
    int i, j;
    int sum;
    printf("thread numero %d\n", k);
    m2 = (int **) malloc(RowA* sizeof(int *));
   for (i = 0; i < RowA; i++)
        m2[i] = malloc(ColB* sizeof(int));
    for (i = 0; i <ColB; i++) {
        sum=0;
        for (j = 0; j < ColA; j++) {
            sum = sum + m[i][k] * m1[k][j];
            m2[i][j] = sum;
        }
        printf("\n");
    }

    pthread_exit(NULL);



}

int main() {
   pthread_t threads[RowA];
    int rc;
    int th;
    int sum;
    double debut,fin;
    char rep;
    srand(time(NULL));
    remplir();
    printf("--------------------le calcul du produit matricielle parallele--------------------------\n");
    debut = clock();


    for(th = 0; th < RowA ; th++){

        rc = pthread_create(&threads[th], NULL, multiplication, th);

        if (rc){
            printf("Erreur de creation de thread; code erreur = %d\n", rc);
            exit(-1);
        }
    } for(th = 0; th < RowA; th++){
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
