#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool creates_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int i;

    for (i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i, j, ind1, ind2;

    for (i = 0; i < candidate_count - 1; i++)
    {
        for (j = 0; j < candidate_count - i - 1; j++)
        {
            ind1 = ranks[i];
            ind2 = ranks[i + 1 + j];
            preferences[ind1][ind2] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int i, j, tmp1, tmp2;

    for (i = 0; i < candidate_count; i++)
    {
        for (j = 0; j < candidate_count - i - 1; j++)
        {
            tmp1 = preferences[i][j + i + 1];
            tmp2 = preferences[j + i + 1][i];
            if (tmp1 < tmp2)
            {
                pairs[pair_count].winner = j + i + 1;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
            else if (tmp1 > tmp2)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j + i + 1;
                pair_count += 1;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i, j, tmp1, tmp2;
    int found = 0;
    pair tmp;

    for (i = 0; i < pair_count; i++)
    {
        found = 0;
        for (j = 0; j < pair_count - i - 1; j++)
        {
            // Use the correct indices to access preferences array
            int winner1 = pairs[j].winner;
            int loser1 = pairs[j].loser;
            tmp1 = preferences[winner1][loser1] - preferences[loser1][winner1];

            int winner2 = pairs[j + 1].winner;
            int loser2 = pairs[j + 1].loser;
            tmp2 = preferences[winner2][loser2] - preferences[loser2][winner2];

            if (tmp1 < tmp2)
            {
                tmp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tmp;
                found = 1;
            }
        }
        if (!found)
            break;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Iterate through each pair and lock it if it doesn't create a cycle
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Check if locking the pair creates a cycle
        if (!creates_cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
}

// Function to check if locking the given pair creates a cycle
bool creates_cycle(int winner, int loser)
{
    // If loser is already a direct or indirect ancestor of the winner, return true
    if (locked[loser][winner])
    {
        return true;
    }

    // Iterate through each candidate and check if the loser is an ancestor of the winner
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner] && creates_cycle(i, loser))
        {
            return true;
        }
    }

    // If no cycle is found, return false
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_winner = true;

        // Check if the candidate is the winner by verifying no incoming edges
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_winner = false;
                break;
            }
        }

        // If no incoming edges, print the winner
        if (is_winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}