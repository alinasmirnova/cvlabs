#include "models.h"

Models::Models(vector<shared_ptr<Descriptor> > desc1, vector<shared_ptr<Descriptor> > desc2)
{
    matches = vector<pair<shared_ptr<Descriptor>, shared_ptr<Descriptor>>>();

    int closestNum;
    for(int i=0; i<desc1.size(); i++)
    {
        closestNum = desc1[i]->findClosest(desc2);
        if(closestNum != -1)
        {
            matches.push_back(make_pair(desc1[i], desc2[closestNum]));
        }
    }
    qDebug() << matches.size();
}

void Models::RANSAAK(int iterCount)
{
    if(matches.size() < 4 || iterCount == 0) return;

    int iter = 0;


    mt19937 mt_rand(time(0));
    uniform_int_distribution<int> rnd(0, matches.size() - 1);

    int baseMatches[4];
    bool anyOverlap;
    int overlap;

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
                for(int j=i; j<4; j++)
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

        gsl_matrix *A = gsl_matrix_alloc(8,9);
        //find inliners count
    }
}

Models::~Models()
{

}

