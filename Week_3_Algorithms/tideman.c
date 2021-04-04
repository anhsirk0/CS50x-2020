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
}
pair;

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i ;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                int a = preferences[i][j];
                int b = preferences[j][i];

                if (a > b)
                {
                    pair new;
                    new.winner = i;
                    new.loser = j;
                    pairs[pair_count++] = new;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strength();
    void swap_pair();
    int n = sizeof(pairs) / sizeof(pairs[0]);
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strength(j) < strength(j + 1))
            {
                swap_pair(&pairs[j], &pairs[j + 1]);
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    bool cycle();
    for (int i = 0; i < pair_count; i++)
    {
        int x = pairs[i].winner;
        int y = pairs[i].loser;
        locked[x][y] = true;
//         check for cycles
        if (cycle(x))
        {
            locked[x][y] = false;
        }
    }
}

bool cycle(int index)
{
    bool already[candidate_count];
    bool is_cycle();
    for (int i = 0; i < candidate_count; i++)
    {
        already[i] = false;
    }
    return is_cycle(index, already);
}

bool is_cycle(int i, bool already[])
{
    if (already[i])
    {
        return true;
    }
    already[i] = true;
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[i][j] && is_cycle(j, already))
        {
            return true;
        }
    }
    return false;
}
// Print the winner of the election
void print_winner(void)
{
    int winner();
    int win = winner();
    if (win != - 1)
    {
        printf("%s\n", candidates[win]);
    }
    return;
}

int strength(int j)
{
    int win = pairs[j].winner;
    int lose = pairs[j].loser;
    int a = preferences[win][lose] - preferences[lose][win];
    return a;
}

void swap_pair(pair *x, pair *y)
{
    pair temp = *x;
    *x = *y;
    *y = temp;
}

int winner(void)
{
    bool source();
    for (int j = 0; j < candidate_count; j++)
    {
        if (source(j))
        {
            return j;
        }
    }
    return - 1;
}

bool source(int j)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][j])
        {
            return false;
        }
    }
    return true;
}
