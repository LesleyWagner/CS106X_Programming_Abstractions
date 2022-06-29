#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
#include "sparsegrid.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/*
 * recursive helper function for canAllPatientsBeSeen
 */
bool patientsHelper(Vector<Doctor>& doctors, Vector<Patient>& patients, Map<string, Set<string>>& schedule) {
    if (patients.isEmpty()) {
        return true;
    }
    else {
        Patient nextPatient = patients[patients.size()-1];
        for (Doctor& doc : doctors) {
            if (doc.hoursFree >= nextPatient.hoursNeeded) {
                doc.hoursFree -= nextPatient.hoursNeeded;
                schedule[doc.name].add(nextPatient.name);
                patients.removeBack();
                if (patientsHelper(doctors, patients, schedule)) {
                    return true;
                }
                doc.hoursFree += nextPatient.hoursNeeded;
                schedule[doc.name].remove(nextPatient.name);
                patients.add(nextPatient);
            }
        }
        return false;
    }
}
/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
    Vector<Doctor> updated_doctors = doctors;
    Vector<Patient> updated_patients = patients;
    for (Doctor& doctor : doctors) {
        schedule.put(doctor.name, {});
    }
    return patientsHelper(updated_doctors, updated_patients, schedule);
}

/* * * * Disaster Planning * * * */

// helper function for canBeMadeDisasterReady
bool disasterHelper(Map<string, Set<string>>& network, int numCities, int citiesNeeded, Set<string>& uncoveredCities, Set<string>& locations) {
    if (numCities < 0) {
        return false;
    }
    else if (uncoveredCities.isEmpty()) {
        return true;
    }
    else if (numCities >= citiesNeeded) {
        string city = uncoveredCities.front();
        Set<string> cities = network[city];
        cities.add(city);
        for (string& choice : cities) {
            if (!uncoveredCities.contains(choice)) {
                continue;
            }
            int newNeeded = 0;
            Set<string> neighbors;
            for (string& neighbor : network[choice]) {
                if (uncoveredCities.contains(neighbor)) {
                    neighbors.add(neighbor);
                }
            }
            for (string& key : network.keys()) {
                if (network[key].isSubsetOf(neighbors) && uncoveredCities.contains(key) && !neighbors.contains(key)) {
                    newNeeded += 1;
                }
            }
            neighbors.add(choice);
            uncoveredCities.removeAll(neighbors);
            // network.remove(choice);
            locations.add(choice);
            if (disasterHelper(network, numCities-1, newNeeded, uncoveredCities, locations)) {
                return true;
            }
            // network.put(choice, neighbors);
            uncoveredCities.addAll(neighbors);
            locations.remove(choice);
        }
        return false;
    }
    else {
        return false;
    }
}
/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
    Map<string, Set<string>> updatedNetwork = roadNetwork;
    Set<string> uncoveredCities;
    int citiesNeeded = 0;
    for (string& city : roadNetwork.keys()) {
        if (roadNetwork[city].isEmpty()) {
            citiesNeeded += 1;
        }
        uncoveredCities.add(city);
    }
    return disasterHelper(updatedNetwork, numCities, citiesNeeded, uncoveredCities, locations);
}

/* * * * DNA Detective * * * */

// helper function for approximatelyMatch
bool dnaHelper(string one, string two, int maxDistance) {
    cout << one << ", " << two << "\n\n";
    int oneLength = one.length();
    int twoLength = two.length();
    bool possible = twoLength-oneLength <= maxDistance;
    if (one == two) {
        return true;
    }
    else if (maxDistance > 0 && possible) {
        bool result = false;
        if (stringContains(two, one)) {
            if (possible) {
                result = true;
            }
            else {
                result = false;
            }
        }
        else {
            string newOne = one;
            string newTwo = two;
            if (newOne.length() == 1) {
                newOne[0] = two[0];
            }
            else {
                // delete
                if (one[1] == two[0]) {
                    newOne.erase(0,1);
                }
                // change
                else if (one[1] == two[1]) {
                    newOne[0] = two[0];
                }
                // prepend
                else {
                    newOne = two[0]+newOne;
                }
            }
            int i = 0;
            while (i < newOne.length()) {
                if (newOne[i] != newTwo[i]) {
                    break;
                }
                i++;
            }
            newOne.erase(0,i);
            newTwo.erase(0,i);
            result = dnaHelper(newOne, newTwo, maxDistance-1);
        }
        return result;
    }
    else {
        return false;
    }
}

/**
 * Given two DNA strands and a maximum edit distance, returns whether those two strands are
 * within that edit distance of one another.
 *
 * @param one The first DNA strand.
 * @param two The second DNA strand.
 * @param maxDistance The maximum distance between them.
 * @return Whether the two strands are within that edit distance of one another.
 */
bool approximatelyMatch(const string& one, const string& two, int maxDistance) {
    if (maxDistance < 0) {
        throw maxDistance;
    }
    string shortest;
    string longest;
    if (two.length() > one.length()) {
        shortest = one;
        longest = two;
    }
    else {
        shortest = two;
        longest = one;
    }
    int i = 0;
    while (i < shortest.length()) {
        if (one[i] != two[i]) {
            break;
        }
        i++;
    }
    shortest.erase(0,i);
    longest.erase(0,i);
    return dnaHelper(shortest, longest, maxDistance);
}

/* * * * Winning the Election * * * */

/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 */
MinInfo minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex, int electoralVotesRemaining) {
    static SparseGrid<MinInfo> cache(states.size(), electoralVotesNeeded+1);
    if (electoralVotesNeeded > electoralVotesRemaining) {
        return MinInfo{INT_MAX, {}};
    }
    else if (minStateIndex == states.size() || electoralVotesNeeded <= 0) {
        return MinInfo{0, {}};
    }
    else if (cache.isSet(minStateIndex, electoralVotesNeeded)) {
        return cache[minStateIndex][electoralVotesNeeded];
    }
    MinInfo minimal{INT_MAX, {}};
    int votesRemaining = electoralVotesRemaining;
    for (int i = minStateIndex; i < states.size(); i++) {
        State state = states[i];
        votesRemaining -= state.electoralVotes;
        MinInfo info;
        info = minPopularVoteToGetAtLeast(electoralVotesNeeded-state.electoralVotes, states, i+1, votesRemaining);
        if (info.popularVotesNeeded != INT_MAX) {
            info.popularVotesNeeded += (state.popularVotes/2)+1;
            info.statesUsed.add(state);
        }
        if (info.popularVotesNeeded < minimal.popularVotesNeeded) {
            minimal = info;
        }
    }
    cache[minStateIndex][electoralVotesNeeded] = minimal;
    return minimal;
}
/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
MinInfo minPopularVoteToWin(const Vector<State>& states) {
    int totalElectoralVotes = 0;
    int electoralVotesNeeded = 0;
    for (State& state : states) {
        totalElectoralVotes += state.electoralVotes;
    }
    electoralVotesNeeded = (totalElectoralVotes/2)+1;
    return minPopularVoteToGetAtLeast(electoralVotesNeeded, states, 0, totalElectoralVotes);
}
