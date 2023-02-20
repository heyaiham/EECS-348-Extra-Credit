#include <map>
#include <array>
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

bool any_false(std::array<bool, 5> array)
{
    for (int i = 0; i < 5; i++)
    {
        if (array[i] == false)
        {
            return true;
        }
    }
    return false;
}

int first_false(std::array<bool, 5> programmerStatus)
{
    for (int i = 0; i < 5; i++)
    {
        if (programmerStatus[i] == false)
        {
            return i;
        }
    }
    return 0;
}

void populate_data(std::array<std::array<int, 5>, 5> & departmentPrefs, std::array<std::array<int, 5>, 5> & programmerPrefs)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if (!input)
    {
        throw std::runtime_error("File failed to open");
    }

    size_t len = 0;
    std::string line;
    int i = 0;
    while ((getline(input, line)))
    {
        if (i < 5) // Department prefs are in the first five lines
        {
            int l1 = 0; // Column
            for (int j1 = 0; j1 < line.length(); j1++)
            {
                if (isdigit(line[j1]))
                {
                    departmentPrefs[l1][i] = (line[j1] - '0');
                    l1++;
                }
            }
        }
        else if (i < 10) // Programmer prefs are in the last five lines
        { 
            int l2 = 0; // Column
            for (size_t j2 = 0; j2 < line.length(); j2++)
            {
                if (isdigit(line[j2]))
                {
                    programmerPrefs[l2][i - 5] = (line[j2] - '0');
                    l2++;
                }
            }
        }
        i++;
    }
    input.close();
}

int main()
{
    printf("Starting the program\n");

    // Make 2D arrays for the people's preferences and populate the data
    std::array<std::array<int, 5>, 5> departmentPrefs;
    std::array<std::array<int, 5>, 5> programmerPrefs;
    populate_data(departmentPrefs, programmerPrefs);

    std::array<bool, 5> departmentStatus = {false};
    std::array<bool, 5> programmerStatus = {false};
    std::map<int, int> departmentAssignment;
    std::map<int, int> programmerAssignment;
    while (any_false(programmerStatus))
    {
        int programmer = first_false(programmerStatus);
        std::array<int, 5> preferences;
        for (int i = 0; i < 5; i++)
        {
            preferences[i] = programmerPrefs[programmer][i];
        }

        for (auto elem: preferences)
        {
            int department = elem - 1;
            if (!departmentStatus[department])
            {
                programmerAssignment[programmer] = department;
                departmentAssignment[department] = programmer;
                programmerStatus[programmer] = true;
                departmentStatus[department] = true;
                break;
            }
            else
            {
                int cur_programmer = departmentAssignment[department];
                int cur_preference = departmentPrefs[department][cur_programmer];
                if (preferences[elem - 1] < cur_preference)
                {
                    programmerAssignment[programmer] = department;
                    departmentAssignment[department] = programmer;
                    programmerStatus[programmer] = true;
                    programmerStatus[cur_programmer] = false;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 5; i++)
    {
        printf("Department #%d will get programmer #%d\n", i + 1, departmentAssignment[i] + 1);
    }

    return 0;
}
