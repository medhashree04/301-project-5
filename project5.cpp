/*
 * CSC-301
 * project5.cpp
 * Fall 2025
 *
 * Partner 1: Medhashree Adhikari
 * Partner 2: Slok Rajbhandari
 * Date: Saturday, December 13, 2025
 */

#include "project5.hpp"

using namespace std;

/**
 * ED - solves the given ED problem using DP or Memoization.
 * 
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      bool useASM: boolean to determine whether perform Edit Distance
 *      bool useDP: boolean to determine whether use DP or Memoization
 * OUTPUT
 *      vector<int> table: table of ints storing minimum number of edits
 */
vector<int> ED(string &src, string &dest, bool useASM, bool useDP) {
    vector<int> table;
    if (useDP) {
        table = ED_DP(src, dest, useASM);
    } else {
        table = ED_Mem(src, dest, useASM);
    }
    return table;
}

/**
 * ED_DP - create and fill the table iteratively using dynamic programming.
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      bool useASM: boolean to determine whether perform Edit Distance
 * 
 * OUTPUT
 *      vector<int> table: table of ints storing minimum number of edits
 */
vector<int> ED_DP(string &src, string &dest, bool useASM) {
    // Get string lengths for ease of use.
    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;

    // Setup the table, size (n+1)x(m+1), filled with -1s.
    vector<int> table( (n+1)*(m+1), -1 );

    // base cases: 1st col
    for(int i = 0; i <= n; i++) {
        table[i*numCols+0] = i;
    }

    if(useASM) { // ASM is true
        for(int j = 0; j<=m; j++) {
            table[0*numCols+j] = 0;
        }
    } else { // not ASM
        for(int j = 0; j<=m; j++) {
            table[0*numCols+j] = j;
        }
    } // ASM bool check

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            // characters match
            if(src[i - 1] == dest[j - 1]) {
                table[i * numCols + j] = table[((i - 1) * numCols) + (j - 1)];
            } else {
                // no match - min of del, ins, sub
                int del = table[(i - 1) * numCols + j]; // delete letter
                int ins = table[i * numCols + (j - 1)]; // place a letter
                int sub = table[(i - 1) * numCols + (j - 1)]; // change a letter
                table[i * numCols + j] = 1 + min({del, ins, sub});
            }
        } // inner loop
    } // outer loop

    // Return the completed table.
    return table;
}

/**
 * ED_Mem - create and fill the table recursively using memoization.
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      bool useASM: boolean to determine whether perform Edit Distance
 * OUTPUT
 *      vector<int> table: table of ints storing minimum number of edits
 */
vector<int> ED_Mem(string &src, string &dest, bool useASM) {
    // Get string lengths for ease of use.
    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;

    // Setup the table, size (n+1)x(m+1), filled with -1s.
    vector<int> table( (n+1)*(m+1), -1 );
    ED_Mem_helper(src, dest, useASM, numCols, n, m, table);

    // Return the completed table.
    return table;
}

/**
 * ED_Mem_helper - recursive helper function for memoization.
 *                 Note: numCols input for ease of indexing.
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      bool useASM: boolean to determine whether perform Edit Distance
 *      int numCols: the number of columns plus 1
 *      int i: index i
 *      int j: index j
 *      vector<int> &table: the table itself.
 * OUTPUT
 *      int: computes & returns the element in table[i * numCols + j]
 */
int ED_Mem_helper(string &src, string &dest, bool useASM, int numCols,
                  int i, int j, vector<int> &table) {
    
    if(table[i * numCols + j] != -1) {
        return table[i * numCols + j];
    } // already done check

    // base cases
    if(i == 0) {
        if(useASM) { // ASM is true
            table[i * numCols + j] = 0;
            return table[i * numCols + j];
        } else { // not ASM
            table[i * numCols + j] = j;
            return table[i * numCols + j];
        } // ASM bool check
    }
    
    if(j == 0) {
            table[i * numCols + j] = i;
            return table[i * numCols + j];
    }

    // recursive case
    if(src[i - 1] == dest[j - 1]) {
        // characters match
        table[i * numCols + j] = ED_Mem_helper(
                src, dest, useASM, numCols, i - 1, j - 1, table
            );
    } else {
        int del = ED_Mem_helper(
            src, dest, useASM, numCols, i - 1, j, table
        ); // delete a character
        int ins = ED_Mem_helper(
            src, dest, useASM, numCols, i, j - 1, table
        ); // insert a character
        int sub = ED_Mem_helper(
            src, dest, useASM, numCols, i - 1, j - 1, table
        ); // change a character

        table[i * numCols + j] = 1 + min({del, ins, sub});
    } // no match - min of del, ins, sub

    return table[i * numCols + j];
}

/**
 * getNumEdits - given a filled table, return the min number of edits.
 * 
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      vector<int> &table: the table itself
 * OUTPUTS
 *      int: minimum number of edits needed to convert src into dest
 */
int getNumEdits(string &src, string &dest, vector<int> &table) {
    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;
    
    return table[n * numCols + m]; // int numEdits 
}

/**
 * getEdits - reconstruct the optimal edits given a filled table.
 * 
 * INPUTS
 *      string &src: string 1 to be changed
 *      string &dest: string 2 not to be changed
 *      vector<int> &table: the table itself
 * OUTPUTS
 *      vector<Edit>: minimum set of edits needed to convert src into dest
 */
vector<Edit> getEdits(string &src, string &dest, vector<int> &table) {
    vector<Edit> edits;

    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;

    int i = n;
    int j = m;

    while((i > 0) or (j > 0)) {
        if(j == 0) {
            edits.push_back(Edit("del", src[i - 1], '\0', i - 1));
            i--;
            continue;
        } // delete
        
        if(i == 0) { 
            edits.push_back(Edit("ins", '\0', dest[j - 1], i));
            j--;
            continue;
        } // insert
        
        if (src[i - 1] == dest[j - 1]) {
            edits.push_back(Edit("match", src[i - 1], dest[j - 1], i - 1));
            i--;
            j--;
            continue;
        } // match

        int del = table[(i - 1) * numCols + j];
        int ins = table[i * numCols + (j - 1)];
        int sub = table[(i - 1) * numCols + (j - 1)];
        
        int minEdits = min({del, ins, sub});

        if(minEdits == del) {
            edits.push_back(Edit("del", src[i - 1], '\0', i - 1));
            i--;
        } else if(minEdits == ins) {
            edits.push_back(Edit("ins", '\0', dest[j - 1], i));
            j--;
        } else {
            edits.push_back(Edit("sub", src[i - 1], dest[j - 1], i - 1));
            i--;
            j--;
        }
    } // look until no more

    return edits;
}

/**
 * printTable: prints the table so it looks nice.
 *
 * INPUTS
 * string src: the source string for ED (or ASM).
 * string dest: the destination string for ED (or ASM).
 * vector<int> &table: the table itself.
 *
 * OUTPUTS
 * string tableString.str(): the pretty string.
 */
string printTable(string &src, string &dest, vector<int> &table) {
    // Create the stringstream for the table.
    stringstream tableString;

    // Shift the first row for the dest letters and write the letters.
    // tableString << setw(labelWidth) << fixed;
    tableString << " " << " " << " " << " ";
    for (int j=0; j<dest.length(); j++) {
        // tableString << setprecision(prec) << setw(numWidth) << fixed;
        tableString << dest[j] << " ";
    }
    tableString << endl;

    // Write each line of the table, with the proper src letter at front.
    int entry;
    for (int i=0; i<src.length()+1; i++) {
        // tableString << setw(labelWidth) << fixed;
        if (i > 0) {
            tableString << src[i-1] << " ";
        } else {
            tableString << " " << " ";
        }
        for (int j=0; j<dest.length()+1; j++) {
            // tableString << setprecision(prec) << setw(numWidth) << fixed;
            entry = table[i*(dest.length()+1) + j];
            if (entry > -1) {
                tableString << entry << " "; // filled
            } else {
                tableString << "X" << " "; // unfilled
            }
        }
        tableString << endl;
    }
    tableString << endl;

    return tableString.str();
}