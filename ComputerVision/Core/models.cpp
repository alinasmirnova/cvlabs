#include "models.h"

Models::Models(vector<shared_ptr<Descriptor> > desc1, vector<shared_ptr<Descriptor> > desc2)
{
    matches = vector<pair<shared_ptr<Descriptor>, shared_ptr<Descriptor>>>();

    int closestNum;
    for(uint i=0; i<desc1.size(); i++)
    {
        closestNum = desc1[i]->findClosest(desc2);
        if(closestNum != -1)
        {
            matches.push_back(make_pair(desc1[i], desc2[closestNum]));
        }
    }
    qDebug() << matches.size();
}

float* Models::RanSaC(int iterCount, float eps)
{
    if(matches.size() < 4 || iterCount == 0) return NULL;

    int iter = 0;


    mt19937 mt_rand(time(0));
    uniform_int_distribution<int> rnd(0, matches.size() - 1);

    int baseMatches[4];
    bool anyOverlap;
    int overlap;

    gsl_matrix *A = gsl_matrix_alloc(8,9);
    gsl_matrix *AT = gsl_matrix_alloc(9,8);
    gsl_matrix *multA = gsl_matrix_alloc(9,9);
    gsl_matrix *V = gsl_matrix_alloc(9, 9);
    gsl_vector *singular = gsl_vector_alloc(9);

    float h[9];
    int inliners;
    float xInit, yInit, xExpect, yExpect, xCur, yCur;
    float lastH;
    int bestInliners = 0;
    float bestModel[9];

    while(iter < iterCount)
    {
        //choose four points
        baseMatches[0] = rnd(mt_rand);
        baseMatches[1] = rnd(mt_rand);
        baseMatches[2] = rnd(mt_rand);
        baseMatches[3] = rnd(mt_rand);

        //we want them to be different
        while(1)
        {
            anyOverlap = false;
            for(int i=0; i<4; i++)
                for(int j=i+1; j<4; j++)
                    if(baseMatches[i] == baseMatches[j])
                    {
                        anyOverlap = true;
                        overlap = j;
                        goto exit;
                    }
            exit: if(anyOverlap)
            {
                baseMatches[overlap] = rnd(mt_rand);
            }
            else break;
        }

        //find model
        //set matrix A
        for(int i=0; i<4; i++)
        {
            gsl_matrix_set(A, i*2, 0, matches[baseMatches[i]].first->point.x);
            gsl_matrix_set(A, i*2, 1, matches[baseMatches[i]].first->point.y);
            gsl_matrix_set(A, i*2, 2, 1);

            gsl_matrix_set(A, i*2, 6, -matches[baseMatches[i]].first->point.x * matches[baseMatches[i]].second->point.x);
            gsl_matrix_set(A, i*2, 7, -matches[baseMatches[i]].first->point.y * matches[baseMatches[i]].second->point.x);
            gsl_matrix_set(A, i*2, 8, -matches[baseMatches[i]].second->point.x);

            gsl_matrix_set(A, i*2 + 1, 3, matches[baseMatches[i]].first->point.x);
            gsl_matrix_set(A, i*2 + 1, 4, matches[baseMatches[i]].first->point.y);
            gsl_matrix_set(A, i*2 + 1, 5, 1);

            gsl_matrix_set(A, i*2 + 1, 6, -matches[baseMatches[i]].first->point.x * matches[baseMatches[i]].second->point.y);
            gsl_matrix_set(A, i*2 + 1, 7, -matches[baseMatches[i]].first->point.y * matches[baseMatches[i]].second->point.y);
            gsl_matrix_set(A, i*2 + 1, 8, -matches[baseMatches[i]].second->point.y);
        }

        gsl_matrix_transpose_memcpy(AT, A);

        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, AT, A, 0, multA);

        gsl_linalg_SV_decomp_jacobi(multA, V, singular);

        //h[8] = 1;
        lastH = gsl_matrix_get(V, 8, 8);
        for(int i=0; i<9; i++)
        {
            h[i] = gsl_matrix_get(V, i, 8)/lastH;
        }

        //find inliners count

        inliners = 0;

        for(uint i=0; i<matches.size(); i++)
        {
            xInit = matches[i].first->point.x;
            yInit = matches[i].first->point.y;

            xExpect = matches[i].second->point.x;
            yExpect = matches[i].second->point.y;

            xCur = (h[0]*xInit + h[1]*yInit + h[2]) / (h[6]*xInit + h[7]*yInit + h[8]);
            yCur = (h[3]*xInit + h[4]*yInit + h[5]) / (h[6]*xInit + h[7]*yInit + h[8]);

            if(sqrt(pow(xExpect - xCur,2)+pow(yExpect - yCur,2)) < eps)
            {
                inliners++;
            }
        }
        if(inliners > bestInliners)
        {
            for(int i=0; i<9; i++)
            {
                bestModel[i] = h[i];
            }
            bestInliners = inliners;
        }
        iter++;
    }

    gsl_matrix_free(A);
    gsl_matrix_free(AT);
    gsl_matrix_free(multA);
    gsl_matrix_free(V);
    gsl_vector_free(singular);

    qDebug() << "Model:";
    for(int i=0; i<9; i++)
    {
         qDebug() << bestModel[i];
    }
    qDebug() <<"Inliers: " << bestInliners;
    return bestModel;
}

Models::~Models()
{

}

