#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <cs50.h>


// number of tabs with numbers
#define NBTAB 6
// number of operations (+-*/)
#define NBOP 4
// substraction for natural ints.
#define DIFF(a, b)       (((a) > (b))? ((a) - (b)):((b) - (a)))

// I need to printf in my string the complete operation to keep track of it, to prinf it later in main
#define SPRINT_OP(a, b, ope, result) ({strcpy(tmp_res, res); sprintf(res,"  %d %c %d = %d\n%s",(((a)>(b))?(a):(b)),c_op[ope],(((a)>(b))?(b):(a)),result,tmp_res);})


int best = 0;
int best_diff = 999;
char res[255];
char tmp_res[255];

int add(int *a, int b)
{
    return(*a += b);
}

int sub(int *a, int b)
{
    return(*a = DIFF(*a, b));
}

int mult(int *a, int b)
{
    return(*a *= b);
}

int divi(int *a, int b)
{
    if( *a > b )
    {
        if(((*a && b) != 0) && (b != 1) && ((*a % b) == 0))
            return(*a = *a / b);
    }
    if( b > *a)
    {
        if(((*a && b) != 0) && (*a != 1) && ((b % *a) == 0))
            return(*a = b / *a);
    }
    return 0;
}

// now I need an array of if int containing all the possible operations
int(*op[])(int* ,int) = {add, sub, mult, divi};
// now I need an array of char for the 4 operators

char c_op[] ="+-*/";

int compute(int* tab, int nb, int total)
{
    int i, j, k; // counters!!
    int t[NBTAB]; // array of int of length the number "given" on the tabs

    // now i will go from 0 to n-1 and j from i+1 to n, regular search nested for loops
    for(i = 0; i < (nb - 1); i++)
        for(j = (i + 1); j < nb; j++)
            // k is the inner loop, for each i and j, we do the 4 op aka NBOP
            for(k = 0; k < NBOP; k++)
            {
                memcpy(t, tab, sizeof(int) * NBTAB); // tab is for stocking, t is for calculus
                if((*op[k])(&t[i], t[j]))
                {
                    if(t[i] == total)
                    {
                        best = total;
                        best_diff = 0;
                        return(SPRINT_OP(tab[i], tab[j], k, t[i]));
                    }
                    if(DIFF(t[i], total) < best_diff)
                    {
                        best = t[i];
                        best_diff = DIFF(best, total);
                    }
                    t[j] = t[nb - 1];
                    if(compute(t, nb - 1, total))
                        return(SPRINT_OP(tab[i], tab[j], k, t[i]));
                }
            }
    return 0;
}

int countdown_timer(void)
{

    unsigned int hours = 0;
    unsigned int minutes = 0;
    unsigned int seconds = 0;
    unsigned int milliseconds = 0;
    unsigned int countdown_time_in_secs = 0, time_left = 0;

    clock_t startTime, countTime;
    countdown_time_in_secs = 30;
    // start clock
    startTime = clock();
    // update timer
    time_left = countdown_time_in_secs - seconds;
    printf("  30 seconds to find the solution.....\n");
    fflush(stdout);

    while (time_left > 0)
    {
        countTime = clock(); // update timer difference
        milliseconds = countTime - startTime;
        seconds = (milliseconds/(CLOCKS_PER_SEC)) - (minutes * 60);
        minutes = (milliseconds/(CLOCKS_PER_SEC)) / 60;
        hours = minutes / 60;
        // subtract to get difference
        time_left = countdown_time_in_secs - seconds;

        //if(time_left % 5 == 0)
        //  printf("\nYou have %d seconds left\n",time_left);
    }
        printf( "           \n  Time's up!!!!\n");
        fflush(stdout);

    return 0;
}

void reset_var(void)
{
  int reset = 0;
  for(reset; reset < 255; reset++)
  {
      res[reset] = '\0';
      tmp_res[reset] = '\0';
  }
  int best = 0;
  int best_diff = 999;
}

char ask(void)
{
  char ok = '\0';
  do
  {
      printf("  Do you want to play again?(type y for yes and n for no)\n");
      fflush(stdout);
      //printf("ok before : %c\n", ok);
      scanf("  %c", &ok);
      fflush(stdout);
      //printf("ok after : %c\n", ok);
  }
  while(ok != 'y' && ok != 'n');
  return ok;
}

int main (void)
{
    int i, num, total, t[NBTAB];
    char ok = '\0';
    int draw[] = {1,2,3,4,5,6,7,8,9,10,25,50,75,100};
    time_t shuffle;
    srand((unsigned) time(&shuffle));
    printf("\n\n************************WELCOME TO THE COUNTDOWN GAME!!!************************\n\n");
    fflush(stdout);
    do
    {
        for (i = 0; i < NBTAB; i++)
        {
            num = draw[rand() % 14];
            printf("  number %d : %d\n",(i+1), num);
            fflush(stdout);
            t[i] = num;
        }
        total = 100 + (rand() % 899);
        printf("\n  total: %d\n\n", total);
        fflush(stdout);
        countdown_timer();

        if(!compute(t, NBTAB, total))
            (compute(t, NBTAB, best));

        if(best == total)
        {
            printf("  We have an answer!\n\n  Solution: \n\n");
            printf("%s \n",res);
            fflush(stdout);
        }
        else
        {
            printf("  No solution was found, but the closest answer is: \n\n");
            printf("  %s \n",res);
            fflush(stdout);
        }
        reset_var();
        ok = ask();
    }
    while(ok == 'y');
    printf("\n\n*******************************GOODBYE!!!************************************\n\n");
    fflush(stdout);
    return EXIT_SUCCESS;
}
