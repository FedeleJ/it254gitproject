/*
Project: Lab07 for CS132
Filename: Lab07.cpp
Author : Jon Fedele
Modified : June 16, 2020

This file reads a .txt file for integers. It uses a one-way linked list and four pointers to sort a set of structures, each
consisting of the input integer and a pointer to the next structure, into a assending list. It then outputs that list to
console. Finally it deletes the list.
*/

#include <iostream>
#include <fstream>
using namespace std;

//Each struct of type EntryInt, i.e. each "entry" consists of an int, numInQ meaning number in question, and a
//pointer to the address of the next entry in the list.
struct EntryInt
{
    int numInQ;
    struct EntryInt *nextEntry;
};

int main()
{
    //This is file input setup.
    ifstream iS;
    iS.open("unsorted127.txt");

    // The following tests whether the input or output streams have opened without error.
    if (iS.fail())
    {
        cout << "Input file opening failed.\n";
        exit(1);
    }

    //This variable keeps track of when we are done with the outer loop, which happens when the stream yields no more ints
    bool stopper = true;
    //This is the variable that the int is stored in before it's ready for its own EntryInt object
    int temp;
    //This pointer points to the start of the list.
    struct EntryInt* headPntr = NULL;
    //This pointer points to the entry just before the current entry we are evaluating within the list.
    struct EntryInt* previousPntr = NULL;
    //This pointer points to the current entry we are evaluating within the list.
    struct EntryInt* currentPntr = NULL;
    //This pointer points to the new entry that does not have a place in the list yet, the enterant.
    struct EntryInt* enterantPntr = NULL;

    //This do loop, refered to as the outer loop, is going to take in as many ints from the file as it can, and put them
    //in a sorted, assending, single-link list.
    do
    {
        //Attempts to withdraw an int from the stream. If successful, will sort int later. If failed, will stop trying.
        if (iS >> temp)
        {
            //Makes a new, nameless object at saves its location at enterantPntr.
            enterantPntr = new EntryInt;
            //Puts the int into the newly declared object, pointed to by enterantPntr.
            (*enterantPntr).numInQ = temp;
            //For now, this object points to nothing, so we'll make a note of that.
            (*enterantPntr).nextEntry = NULL;
            //We'll keep track of whether we've sorted this particular entry with a variable.
            bool qContinue = true;
            //Now we need to determine what this struct object will point to, and what will point to it.
            //Well, if there is no list yet, that's an easy question to answer.
            if (headPntr == NULL)
            {
                //If the list was empty, this entry is the first on the list, at it shall remain pointed at nothing.
                headPntr = enterantPntr;
                qContinue = false;
            }
            //Let's say that this was not the first entry. What then? Examine the first entry. Does the new entry contain
            //an int less than the current head?
            else if ((*enterantPntr).numInQ < (*headPntr).numInQ)
            {
                //If so, the current head is replaced as head by the new entry.
                (*enterantPntr).nextEntry = headPntr;
                headPntr = enterantPntr;
                qContinue = false;
            }
            //Either if the new entry has become the first head of the list or if it has replaced the old head with itself,
            //then it does not need to continue into the loop below: qContinue is false. Otherwise, to the inner loop we go.
            currentPntr = headPntr;
            while (qContinue)
            {
                //Have we reached the end of the list?
                if ((*currentPntr).nextEntry == NULL)
                {
                    //If we've reached the end of the list, then that must be where this new entry goes. Attach it's address
                    //to the entry under examination (the one at currentPntr.)
                    (*currentPntr).nextEntry = enterantPntr;
                    //If we have placed our new entry at the end of this list, we are done with this loop.
                    qContinue = false;
                }
                //But if it wasn't the end of the list, it must point to another entry.
                else
                {
                    //In that case, record the current pointer in case we have to go back to it, and discover the new pointer.
                    previousPntr = currentPntr;
                    currentPntr = (*previousPntr).nextEntry;
                }
                //We've got our new address, which we know has an entry at it (if qContinue is still true.) Thus, what is the
                //int within the new entry?
                //
                //Compare the new entry's int with that of the current entry.
                if (qContinue && ((*enterantPntr).numInQ <= (*currentPntr).numInQ))
                {
                    //If our recently created entry is less than or equal to the entry under consideration, we've gone too
                    //far, although not really because we've saved the previous pointer. Our new entry should go just to
                    //the left of the entry pointed to by currentPntr.
                    //
                    //To do this, we'll change the previous entry's pointer to point to our new entry, and our new entry to
                    //point to the entry at current pointer.
                    (*previousPntr).nextEntry = enterantPntr;
                    (*enterantPntr).nextEntry = currentPntr;
                    //If we have thus found a place for our new entry, we are done with this inner loop.
                    qContinue = false;
                }
                //This is the end of the inner loop. If we have not found a place for our new entry, we will go to the top, at
                //which we will be directed to check if we're at the end of the list. If not we'll get a new entry and try again.
            }
            //Having reached the end of the inner loop, we will go to the end of the outer loop. At the beginning of the next
            //outer loop and see if the stream has anything else to give us. If not, we will stop the outer do loop. If so, we
            //will repeat this outer loop, including at least one interation of the inner loop.
        }
        else
        {
            stopper = false;
        }
    } while (stopper);

    iS.close();

    //Now we need to read it and output it to console. Should be fairly simple.
    currentPntr = headPntr;
    do 
    {
        cout << (*currentPntr).numInQ << endl;
        previousPntr = currentPntr;
        currentPntr = (*previousPntr).nextEntry;
    } while (((*previousPntr).nextEntry != NULL));

    //Now we need to delete the entries, since they've already been output to the console.
    currentPntr = headPntr;
    stopper = true;
    //As long as the list contains at least one entry...
    if (headPntr != NULL)
    {
        do
        {
            //...then check whether we are at the last entry in the list...
            if (currentPntr == NULL)
            {
                stopper = false;
            }
            //...and if there are, record their address...
            else
            {
                previousPntr = currentPntr;
                currentPntr = (*previousPntr).nextEntry;
                //...deleting the entry after you record its next address...
                delete previousPntr;
            }
            //...repeating this process if you were not at the last entry on the list...
        } while (stopper);
        //...and finishing this process if you just deleted the last entry.
    }
}