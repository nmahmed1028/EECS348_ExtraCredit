#include <map>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <typename DataType>

bool any_false(DataType array[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (array[i] == false)
        {
            return true;
        }
    }
    return false;
}

int first_false(bool p_status[5])
{
    for (int i = 0; i < 5; i++)
    {
        if (p_status[i] == false)
            return i;
    }
    return 0;
}

void populate(int (&d_preference)[5][5], int (&p_preference)[5][5])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("matching-data.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);


    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i < 5)
        { 
            int l1 = 0;
            for (int j1 = 0; j1 < read; j1++)
            {
                if (isdigit(line[j1]))
                {
                    d_preference[l1][i] = (line[j1] - '0');
                    l1++;
                }
            }
        }
        else if (i < 10)
        { 
            int l2 = 0;
            for (size_t j2 = 0; j2 < read; j2++)
            {
                if (isdigit(line[j2]))
                {
                    p_preference[l2][i - 5] = (line[j2] - '0');
                    l2++;
                }
            }
        }
        i++;
    }
}

int main()
{
    int num_programmers = 5;
    int num_departments = 5;

    int d_preference[5][5];
    int p_preference[5][5];

    populate(d_preference, p_preference);

    bool d_status[5] = {false};
    bool p_status[5] = {false};

    map<int, int> d_assignment;
    map<int, int> p_assignment;

    while (any_false(p_status, 5))
    {
        int programmer = first_false(p_status);

        int preferences[5];

        for (int i = 0; i < 5; i++)
        {
            preferences[i] = p_preference[programmer][i];
        }

        for (auto i : preferences)
        {
            int department = i - 1;
            if (!d_status[department])
            {
                p_assignment[programmer] = department;
                d_assignment[department] = programmer;
                p_status[programmer] = true;
                d_status[department] = true;
                break;
            }
            else
            {
                int cur_programmer = d_assignment[department];
                int cur_preference = d_preference[department][cur_programmer];
                if (preferences[i - 1] < cur_preference)
                {
                    p_assignment[programmer] = department;
                    d_assignment[department] = programmer;
                    p_status[programmer] = true;
                    p_status[cur_programmer] = false;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 5; i++)
    {
        printf("Department #%d will get programmer #%d\n", i + 1, d_assignment[i] + 1);
    }

    return 0;
}