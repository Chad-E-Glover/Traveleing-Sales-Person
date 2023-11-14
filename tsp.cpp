/*!*****************************************
 * Name: Chad Glover
 * Assignment: Traveling sales person
 * Date : 10/13/2023
 *file: TSP.cpp
 ******************************************/

#include <fstream>
#include <sstream> 
#include "tsp.h"
#include <algorithm>
#include <limits>





struct CityNode
{
  int cost = 0;
  MAP mat = MAP();
	std::vector<int> cityUnvisited{NULL};
  int cityFrom = 0;
  std::vector<int> best_solution_so_far{};
};

std::vector<int> SolveTSPAux(std::vector<CityNode>& ndVec)
{
  while (true)
  {
    // find ind of best cost
    int ndInd = 0;
    int bestCost = std::numeric_limits<int>::max();
    int cityUnvisitedSize = std::numeric_limits<int>::max();
    for (int i = 0; i <static_cast<int>(ndVec.size()); ++i)
    {
      if (
        (ndVec[i].cost == bestCost &&
         static_cast<int>( ndVec[i].cityUnvisited.size()) < cityUnvisitedSize)
        ||
        ndVec[i].cost < bestCost
        )
      {
        bestCost = ndVec[i].cost;
        ndInd = i;
        cityUnvisitedSize = ndVec[i].cityUnvisited.size();
      }
    }
    // get curr CityNode
    CityNode currNd = ndVec[ndInd];

    //termination check (if on last level)
    if (currNd.cityUnvisited.size() == 1)
    {
      currNd.best_solution_so_far.push_back(currNd.cityUnvisited[0]);
      currNd.best_solution_so_far.push_back(0);
      return currNd.best_solution_so_far;
    }

    int const citySize = currNd.mat.size();
 

    for (auto&& cityTo : currNd.cityUnvisited)
    {
      
      MAP subMat(citySize);
      int reducedCost = 0;
      {
       
        for (int i = 0; i < citySize; ++i)
        {
          subMat[i] = currNd.mat[i];
        }

        if (cityTo)
        {
        
          for (auto&& elem : subMat[currNd.cityFrom])
          {
            elem = std::numeric_limits<int>::max();
          }
          for (auto&& row : subMat)
          {
            row[cityTo] = std::numeric_limits<int>::max();
          }

          // no return from city-to to city-from -> infinity
          subMat[cityTo][currNd.cityFrom] = std::numeric_limits<int>::max();



        }

        // do row calc
        for (auto&& vector : subMat)
        {
          // find row min
          std::vector<int>::iterator forwardIt =
            std::min_element(vector.begin(), vector.end());
          int minPos = std::distance(vector.begin(), forwardIt);
          int rowMin = vector[minPos];

          // skip min 0
          if (rowMin == 0 || rowMin > 9999)
          {
            continue;
          }

          // subt min from row
          for (auto&& elem : vector)
          {
            elem -= rowMin;
          }

          // add all min val
          reducedCost += rowMin;
        }

        for (int i = 0; i < citySize; ++i)
        {
          
          std::vector<int> tmpVec{};
          for (auto&& vector : subMat)
          {
            tmpVec.push_back(vector[i]);
          }

          // find col min
          std::vector<int>::iterator forwardIt =
            std::min_element(tmpVec.begin(), tmpVec.end());
          int minPos = std::distance(tmpVec.begin(), forwardIt);
          int colMin = tmpVec[minPos];

          // skip min 0
          if (colMin == 0 || colMin > 9999)
          {
            continue;
          }

          // subt min from col
          for (auto&& vector : subMat)
          {
            vector[i] -= colMin;
          }

          // add all min val
          reducedCost += colMin;
        }
      }



      CityNode tripNode;


      tripNode.best_solution_so_far = currNd.best_solution_so_far;
      tripNode.best_solution_so_far.push_back(cityTo);
      tripNode.cityFrom = cityTo;
      tripNode.mat = subMat;

      tripNode.cityUnvisited = currNd.cityUnvisited;
      tripNode.cityUnvisited.erase(
        std::remove(
          tripNode.cityUnvisited.begin(),
          tripNode.cityUnvisited.end(),
          cityTo
        ),
        tripNode.cityUnvisited.end()
      );

      // 1st city cost
      if (!cityTo)
      {
        tripNode.cost = reducedCost;
      }
      else
      {
        tripNode.cost =
          currNd.mat[currNd.cityFrom][cityTo] + currNd.cost + reducedCost;
      }

      ndVec.push_back(tripNode);

      // break if 1st city
      if (!cityTo)
      {
        break;
      }
    }

    // rm curr CityNode
    ndVec.erase(ndVec.begin() + ndInd);
  }
}

std::vector<int> SolveTSP(char const* filename)
{
  // file and str stream
  std::stringstream ss{};
  {
    // open up file
    std::ifstream ifs(filename);
    if (!ifs.is_open())
    {
      //abort();
      return {};
    }

    // read in
    ss << ifs.rdbuf();

    // close file
    ifs.close();
  }

  // get city size
  int citySize = 0;
  ss >> citySize;

  MAP cityMatrix(citySize);
  {
    
    for (int i = 0; i < citySize; ++i)
    {
      cityMatrix[i].resize(
        citySize, std::numeric_limits<int>::max()
      );
    }

    // make diagonally symmetric mat
    int i = 0;
    while (i < citySize)
    {
      int j = i;
      while (j < citySize)
      {
        // skip diagonal elem
        if (i == j)
        {
          ++j;
          continue;
        }

       
        ss >> cityMatrix[i][j];
        cityMatrix[j][i] = cityMatrix[i][j];

        ++j;
      }
      ++i;
    }
  }


  std::vector<CityNode> ndVec;

  CityNode firstNd;
  firstNd.mat = cityMatrix;
  for (int i = 0; i < citySize; ++i)
  {
    firstNd.cityUnvisited.push_back(i);
  }

  ndVec.push_back(firstNd);

  std::vector<int> res = SolveTSPAux(ndVec);
  return res;
}