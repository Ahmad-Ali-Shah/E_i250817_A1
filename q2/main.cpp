#include <iostream>

#include "RangeAdd.h"

#include "ProvidedHash.h"
#include "rangeAssign.h"
#include "rangeMax.h"
#include "build.h"
#include "budget.h"


using namespace std;


long long getCheckSum(){


    const int n = 100000, q = 100000;

    const long long MOD = 1000000007;

    long long checksum = 0;


    int adds = 0;
    int assigns = 0;
    int queries = 0;


    rng_seed(2024);


    int *a = new int[n];


    for (int i = 0; i < n; i++)

        a[i] = (int)(rng_next() % 1000);


    build(a, n);


    for (int i = 0; i < q; i++) {


        unsigned long long t = rng_next() % 3;


        int l = (int)(rng_next() % n);

        int r = (int)(rng_next() % n);


        if (l > r) {

            int tmp = l;

            l = r;

            r = tmp;
        }


        if (t == 0) {

            long long d =
                (long long)(rng_next() % 100) - 50;


            rangeAdd(l, r, d);

            adds++;


        }
        else if (t == 1) {


            long long v =
                (long long)(rng_next() % 1000);


            rangeAssign(l, r, v);

            assigns++;


        } 
        else {


            long long res = rangeMax(l, r);


            checksum =
                (
                    checksum * 1000003
                    +
                    ((res % MOD) + MOD)
                ) % MOD;


            queries++;
        }

    }


    cout << "n=" << n
         << " q=" << q
         << " adds=" << adds
         << " assigns=" << assigns
         << " queries=" << queries
         << endl;


    cout << "checksum=" << checksum << endl;


    cout << "buildVisits=" << bildOne::buildVisits << endl;


    cout << "opVisits=" << budget::opVisits
         << " budget=" << 80 * (long long)q
         << " status="
         << (
                budget::opVisits <= 80 * (long long)q
                ? "PASS"
                : "FAIL"
            )
         << endl;


    freeBuild(); 
    delete[] a;


    return checksum;
}


int main(){

    getCheckSum();

    return 0;
}