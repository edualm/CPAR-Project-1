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

int nt = 4, EventSet = 0;

long long values[2];

void OnMult(int m_ar, int m_br) {
    int ret;

#if PAPI
        ret = PAPI_start(EventSet);

        if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;
#endif

    double time1, time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (i = 0; i < m_ar; i++)
        for (j = 0; j < m_ar; j++)
            pha[i * m_ar + j] = (double) 1.0;

    for (i = 0; i < m_br; i++)
        for (j = 0; j < m_br; j++)
            phb[i * m_br + j] = (double) (i+1);

    time1 = omp_get_wtime();

    for (i = 0; i < m_ar; i++) {
        for (j = 0; j < m_br; j++){
            temp = 0;

            for (k = 0; k < m_ar; k++)
                temp += pha[i * m_ar + k] * phb[k * m_br + j];

            phc[i * m_ar + j] = temp;
        }
    }

    time2 = omp_get_wtime();

    free(pha);
    free(phb);
    free(phc);

    #if PAPI

            ret = PAPI_stop(EventSet, values);

            if (ret != PAPI_OK)
                cout << "ERRO: Stop PAPI" << endl;

            cout << "OnMult" << "," << m_ar << "," << m_br << "," << (double)(time2 - time1) << "," << values[0] << "," << values[1] << endl;

            ret = PAPI_reset(EventSet);

            if (ret != PAPI_OK)
                std::cout << "FAIL reset" << endl;

    #endif
}

void OnMultLine(int m_ar, int m_br) {
    int ret;

    #if PAPI
            ret = PAPI_start(EventSet);

            if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;
    #endif

    double time1, time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (i = 0; i < m_ar; i++)
        for (j = 0; j < m_ar; j++)
            pha[i * m_ar + j] = (double) 1.0;

    for (i = 0; i < m_br; i++)
        for (j = 0; j < m_br; j++)
            phb[i * m_br + j] = (double) (i+1);

    time1 = omp_get_wtime();

    for (i=0; i < m_ar; i++) {
        for (k = 0; k < m_ar; k++) {
            for (j = 0; j < m_br; j++)
                phc[i * m_ar + j] += pha[i * m_ar + j] * phb[k * m_br + j];
        }
    }

    time2 = omp_get_wtime();

    free(pha);
    free(phb);
    free(phc);

    #if PAPI

            ret = PAPI_stop(EventSet, values);

            if (ret != PAPI_OK)
                cout << "ERRO: Stop PAPI" << endl;

            cout << "OnMultLine" << "," << m_ar << "," << m_br << "," << (double)(time2 - time1) << "," << values[0] << "," << values[1] << endl;

            ret = PAPI_reset(EventSet);

            if (ret != PAPI_OK)
                std::cout << "FAIL reset" << endl;

    #endif
}

void ParOnMult(int m_ar, int m_br) {
    int ret;

    #if PAPI
            ret = PAPI_start(EventSet);

            if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;
    #endif

    double time1, time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

#pragma omp parallel for private(j) num_threads(nt)
    for (i = 0; i < m_ar; i++)
        for (j = 0; j < m_ar; j++)
            pha[i * m_ar + j] = (double)1.0;

#pragma omp parallel for private(j) num_threads(nt)
    for (i = 0; i < m_br; i++)
        for(j = 0; j < m_br; j++)
            phb[i * m_br + j] = (double) (i+1);

    time1 = omp_get_wtime();

    for(i = 0; i < m_ar; i++) {
        for(j = 0; j < m_br; j++) {
            temp = 0;

#pragma omp parallel for reduction(+:temp) num_threads(nt)
            for (k = 0; k < m_ar; k++) {
                temp += pha[i * m_ar + k] * phb[k * m_br + j];
            }

            phc[i * m_ar + j] = temp;
        }
    }

    time2 = omp_get_wtime();

    free(pha);
    free(phb);
    free(phc);

    #if PAPI

            ret = PAPI_stop(EventSet, values);

            if (ret != PAPI_OK)
                cout << "ERRO: Stop PAPI" << endl;

            cout << "ParOnMult" << "," << m_ar << "," << m_br << "," << (double)(time2 - time1) << "," << values[0] << "," << values[1] << endl;

            ret = PAPI_reset(EventSet);

            if (ret != PAPI_OK)
                std::cout << "FAIL reset" << endl;

    #endif
}

void ParOnMultLine(int m_ar, int m_br) {
    int ret;

    #if PAPI
            ret = PAPI_start(EventSet);

            if (ret != PAPI_OK) cout << "ERRO: Start PAPI" << endl;
    #endif

    double time1, time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

#pragma omp parallel for private(j) num_threads(nt)
    for (i = 0; i < m_ar; i++)
        for (j = 0; j < m_ar; j++)
            pha[i * m_ar + j] = (double) 1.0;

#pragma omp parallel for private(j) num_threads(nt)
    for (i = 0; i < m_br; i++)
        for (j = 0; j < m_br; j++)
            phb[i * m_br + j] = (double) (i+1);

    time1 = omp_get_wtime();

#pragma omp parallel for private(k, j) num_threads(nt)
    for (i=0; i < m_ar; i++) {
        for (k = 0; k < m_ar; k++) {
            for (j = 0; j < m_br; j++) {
                phc[i * m_ar + j] += pha[i * m_ar + j] * phb[k * m_br + j];
            }
        }
    }

    time2 = omp_get_wtime();

    free(pha);
    free(phb);
    free(phc);

    #if PAPI

            ret = PAPI_stop(EventSet, values);

            if (ret != PAPI_OK)
                cout << "ERRO: Stop PAPI" << endl;

            cout << "ParOnMultLine" << "," << m_ar << "," << m_br << "," << (double)(time2 - time1) << "," << values[0] << "," << values[1] << endl;

            ret = PAPI_reset(EventSet);

            if (ret != PAPI_OK)
                std::cout << "FAIL reset" << endl;

    #endif

}

void handle_error(int retval) {
#if PAPI

    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));

#endif

    exit(1);

}

void run_tests() {
    cout << endl << endl << "<-----> OnMult" << endl << endl;

    for (int x = 600; x <= 3000; x += 400)
        OnMult(x, x);

    cout << endl << endl << "<-----> OnMultLine" << endl << endl;

    for (int x = 600; x <= 3000; x += 400)
        OnMultLine(x, x);

    for (int x = 4000; x <= 10000; x += 2000)
        OnMultLine(x, x);

    cout << endl << endl << "<-----> ParOnMult" << endl << endl;

    for (nt = 1; nt <= 4; nt++)
        for (int x = 600; x <= 3000; x += 400)
            ParOnMult(x, x);

    cout << endl << endl << "<-----> ParOnMultLine" << endl << endl;

    for (nt = 1; nt <= 4; nt++)
        for (int x = 600; x <= 3000; x += 400)
            ParOnMultLine(x, x);
}

void init_papi() {
#if PAPI

    EventSet = PAPI_NULL;

    int retval = PAPI_library_init(PAPI_VER_CURRENT);

    if (retval != PAPI_VER_CURRENT && retval < 0) {
        printf("PAPI library version mismatch!\n");

        exit(1);
    }

    if (retval < 0)
        handle_error(retval);

    std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
          << " MINOR: " << PAPI_VERSION_MINOR(retval)
          << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";

    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT)
        std::cout << "FAIL" << endl;

    retval = PAPI_create_eventset(&EventSet);

    if (retval != PAPI_OK)
        cout << "ERRO: create eventset" << endl;

    retval = PAPI_add_event(EventSet,PAPI_L1_DCM );

    if (retval != PAPI_OK)
        cout << "ERRO: PAPI_L1_DCM" << endl;

    retval = PAPI_add_event(EventSet,PAPI_L2_DCM);

    if (retval != PAPI_OK)
        cout << "ERRO: PAPI_L2_DCM" << endl;

#endif
}

int main (int argc, char *argv[]) {
    char c;
    int lin, col, nt = 1;
    int op;
    int ret = 0;

    init_papi();

    op = 1;

    run_tests();

    do {
        cout << endl << "1. Multiplication" << endl;
        cout << "2. Line Multiplication" << endl;
        cout << "3. Parallel Multiplication" << endl;
        cout << "4. Parallel Line Multiplication" << endl;

        cout << "Selection?: ";
        cin >> op;

        if (op == 0)
            break;

        printf("Dimensions: lines columns? ");
        cin >> lin >> col;

        // Start counting

        switch (op) {
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

    } while (op != 0);

#if PAPI

    ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK)
        std::cout << "FAIL destroy" << endl;

#endif

}
