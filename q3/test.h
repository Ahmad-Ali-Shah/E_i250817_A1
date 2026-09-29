#pragma once



#include <cstdio>

#include "rng.h"
#include "function1.h"
#include "function2.h"
#include "function3.h"


class Test{


public:


    ////////////////////////////

    // PUBLIC

    ////////////////////////////


    void run(){


        int selfCheckResult = selfCheck();

        int oracleOk = 0;


        if(selfCheckResult == 1){

            oracleOk = oracleCheck();

        }


        finalTest(oracleOk);

    }


private:


    ////////////////////////////

    // PRIVATE

    ////////////////////////////


    int selfCheck(){


        ////////////////////////////

        // SELF CHECK
        // All-ones 1000 x 1000 image
        // Required count = 333,833,500

        ////////////////////////////


        const int selfR = 1000;

        const int selfC = 1000;


        int* selfImage = new int[selfR * selfC];


        for(int i = 0 ; i < selfR * selfC ; i++){

            selfImage[i] = 1;

        }


        long long selfCount = count_ones_squares(
            selfImage,selfR,selfC
        );


        int selfCheck = 1;


        if(selfCount != 333833500){

            selfCheck = 0;

        }


        delete[] selfImage;


        return selfCheck;

    }


    int oracleCheck(){


        ////////////////////////////

        // ORACLE CROSS CHECK
        // 2,000 random matrices up to 6 x 6

        ////////////////////////////


        int oracleOk = 1;


        rng_seed(2024);


        for(int test = 0 ; test < 2000 ; test++){


            int R = (int)(rng_next() % 6) + 1;

            int C = (int)(rng_next() % 6) + 1;


            int* m = new int[R * C];


            for(int i = 0 ; i < R * C ; i++){


                if((rng_next() % 1000) < 600){

                    m[i] = 1;

                }

                else{

                    m[i] = 0;

                }

            }


            int side = 0;

            int top = 0;

            int left = 0;


            long long area = largest_ones_square(m,R,C,side,top,left );


            long long count = count_ones_squares(m,R,C);


            long long bruteArea = largest_ones_square_brute(m,R,C );


            ////////////////////////////

            // Check area

            ////////////////////////////


            if(area != bruteArea){

                oracleOk = 0;

            }


            ////////////////////////////

            // Check side

            ////////////////////////////


            if(area > 0 && (long long)side * side != area){

                oracleOk = 0;

            }


            ////////////////////////////

            // Verify reported square

            ////////////////////////////


            if(side > 0){


                for(int i = top ; i < top + side ; i++){

                    for(int j = left ; j < left + side ; j++){


                        if(i < 0 || j < 0 || i >= R || j >= C){

                            oracleOk = 0;

                        }

                        else{

                            int index = i * C + j;


                            if(m[index] == 0){

                                oracleOk = 0;

                            }

                        }

                    }

                }

            }


            ////////////////////////////

            // If there is no 1

            ////////////////////////////


            if(area == 0){

                if(side != -1 || top != -1 || left != -1){

                    oracleOk = 0;

                }

            }


            ////////////////////////////

            // Brute-force count

            ////////////////////////////


            long long bruteCount = 0;


            for(int i = 0 ; i < R ; i++){

                for(int j = 0 ; j < C ; j++){


                    for(int k = 1 ; i + k <= R && j + k <= C ; k++){


                        int allValuesOne = 1;


                        for(int l = i ; l < i + k ; l++){

                            for(int n = j ; n < j + k ; n++){


                                int index = l * C + n;


                                if(m[index] == 0){

                                    allValuesOne = 0;

                                }

                            }

                        }


                        if(allValuesOne == 1){

                            bruteCount++;

                        }

                    }

                }

            }


            ////////////////////////////

            // Check count

            ////////////////////////////


            if(count != bruteCount){

                oracleOk = 0;

            }


            delete[] m;


            if(oracleOk == 0){

                break;

            }

        }


        return oracleOk;

    }


    void finalTest(int oracleOk){


        ////////////////////////////

        // FINAL 1000 x 1000 TEST

        ////////////////////////////


        const int R = 1000;

        const int C = 1000;


        rng_seed(2024);


        int* m = new int[R * C];


        for(int i = 0 ; i < R * C ; i++){

            m[i] = (int)(rng_next() % 2); // using current random one for generating the random value

        }


        int side = 0;

        int top = 0;

        int left = 0;


        long long area = largest_ones_square( m,R,C,side,top,left);


        long long count = count_ones_squares(
            m,R,C
        );


        ////////////////////////////

        // Required output

        ////////////////////////////



        //R=<R> C=<C> side=<s> area=<a> topleft=(<top>,<left>) squares=<n> oracle_ok=<Y|N>

        printf("R=%d C=%d side=%d area=%lld topleft=(%d,%d) squares=%lld oracle_ok=%s\n", R,C,side,area,top,left,count,oracleOk ? "Y" : "N");

        // cstdio for printing

        delete[] m;

    }


};


