#include "models.h"

Models::Models(vector<shared_ptr<Descriptor> > desc1, vector<shared_ptr<Descriptor> > desc2)
{
    matches = vector<pair<shared_ptr<Descriptor>, shared_ptr<Descriptor>>>();
    best = vector<pair<Point, Point>>(8);
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

    gsl_matrix_set_zero(A);

    float h[9];
    int inliers;
    float xInit, yInit, xExpect, yExpect, xCur, yCur;
    float lastH;
    int bestInliers = 0;

    while(iter < iterCount)
    {
        //choose four points
        baseMatches[0] = rnd(mt_rand);
        baseMatches[1] = rnd(mt_rand);
        baseMatches[2] = rnd(mt_rand);
        baseMatches[3] = rnd(mt_rand);

        gsl_matrix_set_zero(A);

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
            xInit = matches[baseMatches[i]].first->point.x;
            yInit = matches[baseMatches[i]].first->point.y;

            xExpect = matches[baseMatches[i]].second->point.x;
            yExpect = matches[baseMatches[i]].second->point.y;

            gsl_matrix_set(A, i*2, 0, xInit);
            gsl_matrix_set(A, i*2, 1, yInit);
            gsl_matrix_set(A, i*2, 2, 1);

            gsl_matrix_set(A, i*2, 6, -xExpect*xInit);
            gsl_matrix_set(A, i*2, 7, -xExpect*yInit);
            gsl_matrix_set(A, i*2, 8, -xExpect);

            gsl_matrix_set(A, i*2 + 1, 3, xInit);
            gsl_matrix_set(A, i*2 + 1, 4, yInit);
            gsl_matrix_set(A, i*2 + 1, 5, 1);

            gsl_matrix_set(A, i*2 + 1, 6, -yExpect*xInit);
            gsl_matrix_set(A, i*2 + 1, 7, -yExpect*yInit);
            gsl_matrix_set(A, i*2 + 1, 8, -yExpect);
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

        inliers = 0;

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
                inliers++;
            }
        }
        if(inliers > bestInliers)
        {
            for(int i=0; i<9; i++)
            {
                bestModel[i] = h[i];
            }

            for(int i=0; i<4; i++)
            {
                xInit = matches[baseMatches[i]].first->point.x;
                yInit = matches[baseMatches[i]].first->point.y;

                xCur = (h[0]*xInit + h[1]*yInit + h[2]) / (h[6]*xInit + h[7]*yInit + h[8]);
                yCur = (h[3]*xInit + h[4]*yInit + h[5]) / (h[6]*xInit + h[7]*yInit + h[8]);
                best[i] = make_pair(Point(xInit, yInit, 0, 0), Point(xCur,yCur, 0, 0));
            }
            bestInliers = inliers;
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
    qDebug() <<"Inliers: " << bestInliers;
    return bestModel;
}

float *Models::Hough(float eps)
{
    //дескрипторы искомого объекта - первые в паре
        map<ModelParameter, vector<int>> votes;
        float x, y, angle, scale;
        Point point1, point2;
        int xBean, yBean, aBean, sBean;

        float dx = 4, dy = 4, dangle = 3.14/8, dscale = 0.25;
        float shareBean = 2;

        //строим четырехмерный аккумулятор
        for(uint i=0; i<matches.size(); i++)
        {
           point1 = matches[i].first->point;
           point2 = matches[i].second->point;

           scale = point2.scale / point1.scale;

           angle = (point2.angle - point1.angle)*3.14/180;
           //while(angle < 0) angle += 2*3.14;
           //while(angle > 2*3.14) angle -= 2*3.14;

           x = point1.x * cos(angle) - point1.y * sin(angle);
           y = point1.x * sin(angle) + point1.y * cos(angle);

           x = point2.x - x*scale;
           y = point2.y - y*scale;

           xBean = round(x / dx);
           yBean = round(y / dy);
           aBean = round(angle / dangle);
           sBean = round(scale / dscale);

           for(int sx = -shareBean; sx <=shareBean; sx++)
           {
               for(int sy = -shareBean; sy <=shareBean; sy++)
               {
                   for(int sa = -shareBean; sa <=shareBean; sa++)
                   {
                       for(int ss = -shareBean; ss <=shareBean; ss++)
                       {
                           auto model = ModelParameter(xBean + sx, yBean + sy, sBean + ss, aBean + sa);
                           if(votes.find(model) == votes.end())
                           {
                               votes[model] = vector<int>();
                           }
                           if(find(begin(votes[model]), end(votes[model]), i) == votes[model].end())
                           {
                               votes[model].push_back(i);
                           }
                       }
                   }
               }
           }
        }

        float h[9];
        int inliers;
        float xInit, yInit, xExpect, yExpect, xCur, yCur;
        int bestInliers = -1;


        gsl_matrix *A = gsl_matrix_alloc(6,6);
        gsl_vector *b = gsl_vector_alloc(6);
        gsl_vector *solution = gsl_vector_alloc(6);
        gsl_permutation * p = gsl_permutation_alloc (6);
        int signum;

        for(auto vote = votes.begin(); vote != votes.end(); vote++)
        {
            if(vote->second.size() > 2)
            {
                //qDebug() << vote->second.size();
                for(int i=0; i<3; i++)
                {
                    xInit = matches[vote->second[i]].first->point.x;
                    yInit = matches[vote->second[i]].first->point.y;

                    xExpect = matches[vote->second[i]].second->point.x;
                    yExpect = matches[vote->second[i]].second->point.y;

                    gsl_matrix_set(A, i*2, 0, xInit);
                    gsl_matrix_set(A, i*2, 1, yInit);
                    gsl_matrix_set(A, i*2, 2, 1);

                    gsl_matrix_set(A, i*2 + 1, 3, xInit);
                    gsl_matrix_set(A, i*2 + 1, 4, yInit);
                    gsl_matrix_set(A, i*2 + 1, 5, 1);

                    gsl_vector_set(b, i*2, xExpect);
                    gsl_vector_set(b, i*2 + 1, yExpect);
                }

                gsl_linalg_LU_decomp (A, p, &signum);
                gsl_linalg_LU_solve (A, p, b, solution);

                for(int i=0; i<6; i++)
                {
                    h[i] = gsl_vector_get(solution, i);
                }
                h[6] = 0;
                h[7] = 0;
                h[8] = 1;
                //find inliners count

                inliers = 0;

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
                        inliers++;
                    }
                }
                if(inliers > bestInliers)
                {
                    for(int i=0; i<9; i++)
                    {
                        bestModel[i] = h[i];
                    }

                    bestInliers = inliers;
                }
            }
        }
        gsl_matrix_free(A);
        gsl_vector_free(b);
        gsl_vector_free(solution);
        gsl_permutation_free(p);


        qDebug() << "Model:";
        for(int i=0; i<9; i++)
        {
             qDebug() << bestModel[i];
        }
        qDebug() <<"Inliers: " << bestInliers;
        return bestModel;
}

Models::~Models()
{

}

