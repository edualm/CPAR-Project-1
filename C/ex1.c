#define PAPI 1

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>

#if PAPI
#include <papi.h>
#endif

using namespace std;

#define SYSTEMTIME clock_t


void OnMult(int m_ar, int m_br)
{
    
    SYSTEMTIME Time1, Time2;
    
    char st[100];
    double temp;
    int i, j, k;
    
    double *pha, *phb, *phc;
    
    
    
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));
    
    for(i=0; i<m_ar; i++)
        for(j=0; j<m_ar; j++)
            pha[i*m_ar + j] = (double)1.0;
    
    
    
    for(i=0; i<m_br; i++)
        for(j=0; j<m_br; j++)
            phb[i*m_br + j] = (double)(i+1);
    
    
    
    Time1 = omp_get_wtime();
    
    for(i=0; i<m_ar; i++)
    {	for( j=0; j<m_br; j++)
    {	temp = 0;
        for( k=0; k<m_ar; k++)
        {
            temp += pha[i*m_ar+k] * phb[k*m_br+j];
        }
        phc[i*m_ar+j]=temp;
    }
    }
    
    
    Time2 = omp_get_wtime();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;
    
    cout << "Result matrix: " << endl;
    for(i=0; i<1; i++)
    {	for(j=0; j<min(10,m_br); j++)
        cout << phc[j] << " ";
    }
    cout << endl;
    
    free(pha);
    free(phb);
    free(phc);
    
    
}


void OnMultLine(int m_ar, int m_br)
{
    
    SYSTEMTIME Time1, Time2;
    
    char st[100];
    double temp;
    int i, j, k;
    
    double *pha, *phb, *phc;
    
    
    
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));
    
    for(i=0; i<m_ar; i++)
        for(j=0; j<m_ar; j++)
            pha[i*m_ar + j] = (double)1.0;
    
    
    
    for(i=0; i<m_br; i++)
        for(j=0; j<m_br; j++)
            phb[i*m_br + j] = (double)(i+1);
    
    
    
    Time1 = omp_get_wtime();
    
    for (i=0;  i < m_ar; i++) {
        for (k = 0; k < m_ar; k++) {
            for (j = 0; j < m_br; j++)
                phc[i * m_ar + j] += pha[i * m_ar + j] * phb[k * m_br + j];
        }
    }
    
    Time2 = omp_get_wtime();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;
    
    cout << "Result matrix: " << endl;
    for(i=0; i<1; i++)
    {	for(j=0; j<min(10,m_br); j++)
        cout << phc[j] << " ";
    }
    cout << endl;
    
    free(pha);
    free(phb);
    free(phc);
}

void ParOnMult(int m_ar, int m_br) {
    //    omp_set_num_threads(4);
    
    SYSTEMTIME Time1, Time2;
    
    char st[100];
    double temp;
    int i, j, k;
    
    double *pha, *phb, *phc;
    
    
    
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));
    
#pragma omp parallel for private(j) num_threads(8)
    for(i=0; i<m_ar; i++)
        for(j=0; j<m_ar; j++)
            pha[i*m_ar + j] = (double)1.0;
    
#pragma omp parallel for private(j) num_threads(8)
    for(i=0; i<m_br; i++)
        for(j=0; j<m_br; j++)
            phb[i*m_br + j] = (double)(i+1);
    
    Time1 = omp_get_wtime();
    
#pragma omp parallel private(i, j, temp) num_threads(8)
    for(i=0; i<m_ar; i++) {
        for(j=0; j<m_br; j++) {
            temp = 0;
            
#pragma omp parallel for reduction(+:temp) num_threads(8)
            for( k=0; k<m_ar; k++) {
                temp += pha[i*m_ar+k] * phb[k*m_br+j];
            }
            
            phc[i*m_ar+j] = temp;
        }
    }
    
    
    Time2 = omp_get_wtime();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;
    
    cout << "Result matrix: " << endl;
    for(i=0; i<1; i++)
    {	for(j=0; j<min(10,m_br); j++)
        cout << phc[j] << " ";
    }
    cout << endl;
    
    free(pha);
    free(phb);
    free(phc);
}

void ParOnMultLine(int m_ar, int m_br)
{
    
    SYSTEMTIME Time1, Time2;
    
    char st[100];
    double temp;
    int i, j, k;
    
    double *pha, *phb, *phc;
    
    
    
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));
    
    for(i=0; i<m_ar; i++)
        for(j=0; j<m_ar; j++)
            pha[i*m_ar + j] = (double)1.0;
    
    
    
    for(i=0; i<m_br; i++)
        for(j=0; j<m_br; j++)
            phb[i*m_br + j] = (double)(i+1);
    
    
    
    Time1 = omp_get_wtime();
    
    for (i=0;  i < m_ar; i++) {
        for (k = 0; k < m_ar; k++) {
            for (j = 0; j < m_br; j++) {
#pragma mark critical
                phc[i * m_ar + j] += pha[i * m_ar + j] * phb[k * m_br + j];
            }
        }
    }
    
    Time2 = omp_get_wtime();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;
    
    cout << "Result matrix: " << endl;
    for(i=0; i<1; i++)
    {	for(j=0; j<min(10,m_br); j++)
        cout << phc[j] << " ";
    }
    cout << endl;
    
    free(pha);
    free(phb);
    free(phc);
}

float produtoInterno(float *v1, float *v2, int col)
{
    int i;
    float soma=0.0;
    
    for(i=0; i<col; i++)
        soma += v1[i]*v2[i];
    
    return(soma);
    
}

void handle_error (int retval)
{
#if PAPI
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
#endif
    exit(1);
}

void init_papi() {
#if PAPI
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT && retval < 0) {
        printf("PAPI library version mismatch!\n");
        exit(1);
    }
    if (retval < 0) handle_error(retval);
    
    std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
    << " MINOR: " << PAPI_VERSION_MINOR(retval)
    << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
#endif
}


int main (int argc, char *argv[])
{
    
    char c;
    int lin, col, nt=1;
    int op;
    
#if PAPI
    int EventSet = PAPI_NULL;
#else
    int EventSet = 0;
#endif
    long long values[2];
    int ret;
    
#if PAPI
    ret = PAPI_library_init( PAPI_VER_CURRENT );
    if ( ret != PAPI_VER_CURRENT )
        std::cout << "FAIL" << endl;
    
    
    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK) cout << "ERRO: create eventset" << endl;
    
    
    ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
    if (ret != PAPI_OK) cout << "ERRO: PAPI_L1_DCM" << endl;
    
    
    ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
    if (ret != PAPI_OK) cout << "ERRO: PAPI_L2_DCM" << endl;
#endif
    
    op=1;
    do {
        cout << endl << "1. Multiplication" << endl;
        cout << "2. Line Multiplication" << endl;
        cout << "Selection?: ";
        cin >>op;
        if (op == 0)
            break;
        printf("Dimensions: lins cols ? ");
        cin >> lin >> col;
        
        
        
        // Start counting
#if PAPI
        ret = PAPI_start(EventSet);
        if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;
#endif
        
        switch (op){
            case 1:
                OnMult(lin, col);
                break;
            case 2:
                OnMultLine(lin, col);
                
                break;
                
            case 3:
                ParOnMult(lin, col);
                
                break;
                
            case 4:
                ParOnMultLine(lin, col);
                
                break;
        }
        
#if PAPI
        
        ret = PAPI_stop(EventSet, values);
        if (ret != PAPI_OK) cout << "ERRO: Stop PAPI" << endl;
        printf("L1 DCM: %lld \n",values[0]);
        printf("L2 DCM: %lld \n",values[1]);
        
        ret = PAPI_reset( EventSet );
        if ( ret != PAPI_OK )
            std::cout << "FAIL reset" << endl; 
        
#endif
        
    }while (op != 0);
    
#if PAPI
    ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
    if ( ret != PAPI_OK )
        std::cout << "FAIL remove event" << endl; 
    
    ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
    if ( ret != PAPI_OK )
        std::cout << "FAIL remove event" << endl; 
    
    ret = PAPI_destroy_eventset( &EventSet );
    if ( ret != PAPI_OK )
        std::cout << "FAIL destroy" << endl;
#endif
    
}
