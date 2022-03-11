#include <iostream>
#include <vector>

class MyObject
{
public:
    int key;

    MyObject(int key)
    {
        this->key = key;
    }
};

class Queue
{
private:
    Queue* next;
    Queue* previous;
    Queue* head;
    Queue* tale;

public:
    int length;
    MyObject* obj;

    Queue()
    {
        this->length = 0;
        this->obj = nullptr;
        this->next = nullptr;
        this->previous = nullptr;
        this->head = nullptr;
        this->tale = nullptr;
    }

    Queue(MyObject* obj)
    {
        this->length = 1;
        this->obj = obj;
        this->next = nullptr;
        this->previous = nullptr;
        this->head = this;
        this->tale = this;
    }

    void Push(MyObject* obj)
    {
        Queue* item = new Queue(obj);
        if (this->length > 0)
        {
            this->tale->next = item;
            item->previous = this->tale;
            this->tale = item;
        }
        else
        {
            this->head = item;
            this->tale = item;
            item->obj = obj;
        }
        this->length++;
    }

    MyObject* PopFront()
    {
        if (this->length == 0) return nullptr;

        Queue* popItem = this->head;
        MyObject* result = this->head->obj;
        this->head = this->head->next;
        this->length--;
        delete popItem;
        return result;
    }

    MyObject* PopBack()
    {
        if (this->length == 0) return nullptr;

        Queue* popItem = this->tale;
        MyObject* result = this->tale->obj;
        this->tale = this->tale->previous;
        this->length--;
        delete popItem;
        return result;
    }
};

struct SelectionTreeNode
{
    int key;
    MyObject* record;
    int loser;
    int rn;
    int pe;
    int pi;
};

using namespace std;

bool CreateInitialRuns(int& rq, int& rc, int& rmax, SelectionTreeNode* SelectionTree, int& q, int& lastKey, Queue* tape, Queue* InputData, Queue* Buffer, bool& useBuffer);
void TapesMerge(vector <Queue*> Tapes, Queue* ResultTape, vector <bool> isDirectReading, bool isReverseMerge);
void WriteInitialRunOnTape(vector <vector <int>>& a, vector <int>& d, Queue* Tape, int& rq, int& rc, int& rmax, SelectionTreeNode* SelectionTree, int& Q, int& lastKey, Queue* InputData, Queue* Buffer, bool& useBuffer, bool& isInputDataEnd, int l, int i);
int OscillatingSort(vector <Queue*>& Tapes, Queue* InputData);
int CrissCrossOscillatingSort(vector <Queue*>& Tapes, Queue* InputData);
bool TestOscillatingSort(int a, vector <Queue*>& Tapes);
bool TestCrissCrossOscillatingSort(int a, vector <Queue*>& Tapes);

const int MAX_SIZE = 10000;
int main()
{
    srand(37);
    vector <Queue*> Tapes;
    for (int i = 0; i < 10; i++)
    {
        Tapes.push_back(new Queue());
    }

    cout << TestOscillatingSort(500000, Tapes);
    cout << endl << endl;
    cout << TestCrissCrossOscillatingSort(500000, Tapes);
}

bool CreateInitialRuns(int& rq, int& rc, int& rmax, SelectionTreeNode* SelectionTree, int& q, int& lastKey, Queue* tape, Queue* InputData, Queue* Buffer, bool& useBuffer)
{
    bool isFirstItem = true;
    while (true)
    {
        if (rq != rc)
        {
            if (Buffer->length == MAX_SIZE)
            {
                useBuffer = true;
            }
            if (rc != 0)
            {
                tape->Push(new MyObject(INT32_MAX));
            }
            if (rq > rmax) return true;
            else
            {
                rc = rq;
            }
            if (rc != 1) return false;
        }

        if (rq != 0)
        {
            if (isFirstItem)
            {
                tape->Push(new MyObject(INT32_MAX));
                isFirstItem = false;
            }
            tape->Push(SelectionTree[q].record);
            lastKey = SelectionTree[q].key;
        }

        while (true)
        {
            if (InputData->length == 0 && !useBuffer)
            {
                if (Buffer->length > 0)
                {
                    useBuffer = true;
                    continue;
                }
                else
                {
                    rq = rmax + 1;
                }
            }
            else
            {
                MyObject* record;
                if (useBuffer)
                {
                    if (Buffer->length > 0)
                    {
                        record = Buffer->PopFront();
                        SelectionTree[q].record = record;
                        SelectionTree[q].key = record->key;
                    }
                    else
                    {
                        useBuffer = false;
                        continue;
                    }
                }
                else
                {
                    record = InputData->PopFront();
                    SelectionTree[q].record = record;
                    SelectionTree[q].key = record->key;
                }
                if (record->key < lastKey)
                {
                    if (Buffer->length < MAX_SIZE && !useBuffer)
                    {
                        Buffer->Push(record);
                        continue;
                    }
                    else
                    {
                        rq++;
                        if (rq > rmax)
                        {
                            rmax = rq;
                        }
                        break;
                    }
                }
            }
            break;
        }

        int t = SelectionTree[q].pe;
        while (true)
        {
            if (SelectionTree[t].rn < rq
                || (SelectionTree[t].rn == rq && SelectionTree[SelectionTree[t].loser].key < SelectionTree[q].key))
            {
                int a = q;
                q = SelectionTree[t].loser;
                SelectionTree[t].loser = a;
                if (rq != SelectionTree[t].rn)
                {
                    a = rq;
                    rq = SelectionTree[t].rn;
                    SelectionTree[t].rn = a;
                }
            }

            if (t == 1) break;
            else
            {
                t = SelectionTree[t].pi;
            }
        }
    }
}

void TapesMerge(vector <Queue*> Tapes, Queue* ResultTape, vector <bool> isDirectReading, bool isReverseMerge)
{
    if (Tapes.size() == 1)
    {
        if (!isDirectReading[0])
        {
            ResultTape->Push(Tapes[0]->PopBack());
            while (true)
            {
                MyObject* record = Tapes[0]->PopBack();
                ResultTape->Push(record);
                if (record->key == INT32_MAX) break;
            }
            return;
        }
        else
        {
            ResultTape->Push(Tapes[0]->PopFront());
            while (true)
            {
                MyObject* record = Tapes[0]->PopFront();
                ResultTape->Push(record);
                if (record->key == INT32_MAX) break;
            }
            return;
        }
    }

    bool isFirstItem = true;
    int tapesSize = Tapes.size();
    int firstValue = isReverseMerge ? INT32_MAX : INT32_MIN;
    int lastValue = isReverseMerge ? INT32_MIN : INT32_MAX;
    int q = 0;
    SelectionTreeNode* SelectionTree = new SelectionTreeNode[tapesSize];

    for (int i = 0; i < tapesSize; i++)
    {
        SelectionTree[i] = { firstValue, nullptr, i, 0, (tapesSize + i) / 2, i / 2 };
    }

    for (int i = 0; i < tapesSize; i++)
    {
        if (isDirectReading[i])
        {
            Tapes[i]->PopFront();
        }
        else
        {
            Tapes[i]->PopBack();
        }
    }

    while (true)
    {
        if (SelectionTree[q].key != firstValue)
        {
            if (isFirstItem)
            {
                ResultTape->Push(new MyObject(INT32_MAX));
                isFirstItem = false;
            }
            ResultTape->Push(SelectionTree[q].record);
        }

        MyObject* record;
        if (isDirectReading[q])
        {
            record = Tapes[q]->PopFront();
        }
        else
        {
            record = Tapes[q]->PopBack();
        }

        if (record->key == INT32_MAX)
        {
            SelectionTree[q].key = lastValue;
            tapesSize--;
            if (tapesSize == 0)
            {
                ResultTape->Push(record);
                return;
            }
            delete record;
        }
        else
        {
            SelectionTree[q].key = record->key;
            SelectionTree[q].record = record;
        }

        int t = SelectionTree[q].pe;
        while (true)
        {
            bool isNextRuns = isReverseMerge ? SelectionTree[SelectionTree[t].loser].key > SelectionTree[q].key : SelectionTree[SelectionTree[t].loser].key < SelectionTree[q].key;
            if (isNextRuns)
            {
                int a = q;
                q = SelectionTree[t].loser;
                SelectionTree[t].loser = a;
            }

            if (t == 1) break;
            else t = SelectionTree[t].pi;
        }
    }
}

void WriteInitialRunOnTape(vector <vector <int>>& a, vector <int>& d, Queue* Tape, int& rq, int& rc, int& rmax, SelectionTreeNode* SelectionTree, int& Q, int& lastKey, Queue* InputData, Queue* Buffer, bool& useBuffer, bool& isInputDataEnd, int l, int i)
{
    a[l][i] = 0;
    if (!isInputDataEnd)
    {
        if (CreateInitialRuns(rq, rc, rmax, SelectionTree, Q, lastKey, Tape, InputData, Buffer, useBuffer))
        {
            isInputDataEnd = true;
        }
    }
    else
    {
        d[i]++;
    }
}

int OscillatingSort(vector <Queue*>& Tapes, Queue* InputData)
{
    int tapesSize = Tapes.size();
    int l = 1;
    int lMax = 1;
    int q = 0;
    bool isInputDataEnd = false;
    vector <bool> isDirectReading;
    vector <int> runsOnTape;
    vector <vector <int>> a;
    vector <int> runsCounter;

    isDirectReading.resize(tapesSize);
    runsOnTape.resize(tapesSize);
    int aSize = (int)ceil(log((double)InputData->length / ((double)MAX_SIZE * 2)) / log(tapesSize - 1));
    if (aSize <= 0) aSize = 1;
    a.resize(aSize);
    for (int i = 0; i < a.size(); i++)
    {
        a[i].resize(tapesSize);
    }
    runsCounter.resize(a.size());

    int rmax = 0;
    int rc = 0;
    int rq = 0;
    int Q = 0;
    int lastKey = INT32_MAX;
    bool useBuffer = false;
    SelectionTreeNode SelectionTree[MAX_SIZE];
    Queue* Buffer = new Queue();

    for (int i = 0; i < MAX_SIZE; i++)
    {
        SelectionTree[i] = { INT32_MAX, nullptr, i, 0, (MAX_SIZE + i) / 2, i / 2 };
    }

    a[0][0] = -1;
    for (int i = 1; i < tapesSize; i++)
    {
        if (!isInputDataEnd)
        {
            a[runsOnTape[i]][i] = 0;
            runsOnTape[i]++;

            if (CreateInitialRuns(rq, rc, rmax, SelectionTree, Q, lastKey, Tapes[i], InputData, Buffer, useBuffer)) {
                isInputDataEnd = true;
            }
        }
    }

    while (true)
    {
        if (isInputDataEnd)
        {
            int minRunsIndex = 0;
            int maxRuns;
            bool isReverseMerge = false;

            while (true)
            {
                int j = 0;
                bool isLastStep = true;
                vector <Queue*> TapesToMerge;
                maxRuns = runsOnTape[0];

                for (int i = 1; i < tapesSize; i++)
                {
                    if (runsOnTape[i] > maxRuns)
                    {
                        maxRuns = runsOnTape[i];
                    }

                    if (runsOnTape[i] > 1
                        && ((isReverseMerge && a[runsOnTape[i] - 1][i] % 2 == 1)
                            || (!isReverseMerge && a[runsOnTape[i] - 1][i] % 2 == 0)))
                    {
                        continue;
                    }

                    if (runsOnTape[i] < runsOnTape[minRunsIndex])
                    {
                        if (runsOnTape[minRunsIndex] > 1
                            && ((isReverseMerge && a[runsOnTape[minRunsIndex] - 1][minRunsIndex] % 2 == 1)
                                || (!isReverseMerge && a[runsOnTape[minRunsIndex] - 1][minRunsIndex] % 2 == 0)))
                        {
                            minRunsIndex = i;
                            continue;
                        }

                        if (runsOnTape[minRunsIndex] != 1
                            || (isReverseMerge && a[0][minRunsIndex] % 2 == 0)
                            || (!isReverseMerge && a[0][minRunsIndex] % 2 == 1))
                        {
                            isDirectReading[j++] = false;
                        }
                        else
                        {
                            isDirectReading[j++] = true;
                        }

                        TapesToMerge.push_back(Tapes[minRunsIndex]);
                        runsOnTape[minRunsIndex]--;
                        minRunsIndex = i;
                        continue;
                    }

                    if (runsOnTape[i] == 0) continue;
                    if (runsOnTape[i] != 1
                        || (isReverseMerge && a[0][i] % 2 == 0)
                        || (!isReverseMerge && a[0][i] % 2 == 1))
                    {
                        isDirectReading[j++] = false;
                    }
                    else
                    {
                        isDirectReading[j++] = true;
                    }

                    TapesToMerge.push_back(Tapes[i]);
                    runsOnTape[i]--;
                }

                if (maxRuns == 1 && runsOnTape[minRunsIndex] == 0)
                {
                    TapesMerge(TapesToMerge, Tapes[minRunsIndex], isDirectReading, false);
                    return minRunsIndex;
                }

                if (TapesToMerge.size() != 0) {
                    TapesMerge(TapesToMerge, Tapes[minRunsIndex], isDirectReading, isReverseMerge);
                    a[runsOnTape[minRunsIndex]][minRunsIndex] = isReverseMerge ? 1 : 0;
                    runsOnTape[minRunsIndex]++;
                }

                minRunsIndex = 0;

                for (int i = 0; i < tapesSize; i++)
                {
                    if (runsOnTape[i] > 1)
                    {
                        isLastStep = false;
                        break;
                    }
                }

                isReverseMerge = isLastStep ? false : !isReverseMerge;
            }
        }

        while (true)
        {
            bool isNextLevel = false;
            bool isReverseMerge;
            vector <Queue*> TapesToMerge;

            for (int i = 0; i < tapesSize; i++)
            {
                if (i == q) continue;

                isReverseMerge = (l + 1) % 2 == 0 ? true : false;
                TapesToMerge.push_back(Tapes[i]);
                runsOnTape[i]--;
            }

            TapesMerge(TapesToMerge, Tapes[q], isDirectReading, isReverseMerge);

            a[runsOnTape[q]][q] = l;
            runsOnTape[q]++;
            runsCounter[l]++;
            runsCounter[l - 1] = 0;

            if (runsOnTape[(q + 2) % tapesSize] > 0 && a[runsOnTape[(q + 2) % tapesSize] - 1][(q + 2) % tapesSize] == l)
            {
                isNextLevel = true;
            }
            if (isNextLevel)
            {
                l++;
                q = (q + 1) % tapesSize;
                if (l > lMax) lMax++;
                continue;
            }
            
            q = 0;            
            for (int i = lMax; i > 0; i--)
            {
                q = (q + runsCounter[i]) % tapesSize;
            }
            l = 1;
            break;
        }

        for (int i = (q + 1) % tapesSize; i != q; i = (i + 1) % tapesSize)
        {
            if (!isInputDataEnd)
            {
                a[runsOnTape[i]][i] = 0;
                runsOnTape[i]++;

                if (CreateInitialRuns(rq, rc, rmax, SelectionTree, Q, lastKey, Tapes[i], InputData, Buffer, useBuffer))
                {
                    isInputDataEnd = true;
                    break;
                }
            }
        }
    }
}

int CrissCrossOscillatingSort(vector <Queue*>& Tapes, Queue* InputData)
{
    int tapesSize = Tapes.size();
    int l = 0;
    int q = 0;
    int s;
    int r;
    bool isInputDataEnd = false;
    vector <bool> isDirectReading;
    vector <int> d;
    vector <vector <int>> a;

    isDirectReading.resize(tapesSize);
    d.resize(tapesSize);
    int aSize = (int)ceil(log((double)InputData->length / ((double)MAX_SIZE * 2)) / log(tapesSize - 1));
    if (aSize <= 0) aSize = 1;
    a.resize(aSize);
    for (int i = 0; i < a.size(); i++)
    {
        a[i].resize(tapesSize);
    }

    int rmax = 0;
    int rc = 0;
    int rq = 0;
    int Q = 0;
    int lastKey = INT32_MAX;
    bool useBuffer = false;
    SelectionTreeNode SelectionTree[MAX_SIZE];
    Queue* Buffer = new Queue();

    for (int i = 0; i < MAX_SIZE; i++)
    {
        SelectionTree[i] = { 0, nullptr, i, 0, (MAX_SIZE + i) / 2, i / 2 };;
    }

    a[0][0] = -1;
    for (int i = 1; i < tapesSize; i++)
    {
        WriteInitialRunOnTape(a, d, Tapes[i], rq, rc, rmax, SelectionTree, Q, lastKey, InputData, Buffer, useBuffer, isInputDataEnd, l, i);
    }

    while (true)
    {
        if (isInputDataEnd)
        {
            vector <Queue*> TapesToMerge;
            int j = 0;
            for (int i = 0; i < tapesSize; i++)
            {
                if (i == q || d[i] > 0) continue;

                isDirectReading[j++] = a[0][i] % 2 == 0 ? true : false;
                TapesToMerge.push_back(Tapes[i]);
            }

            TapesMerge(TapesToMerge, Tapes[q], isDirectReading, false);
            return q;
        }

        while (true)
        {
            while (true)
            {
                l++;
                r = q;
                s = 0;
                q = (q + 1) % tapesSize;

                for (int i = 1; i < tapesSize - 1; i++)
                {
                    WriteInitialRunOnTape(a, d, Tapes[(q + i) % tapesSize], rq, rc, rmax, SelectionTree, Q, lastKey, InputData, Buffer, useBuffer, isInputDataEnd, l, (q + i) % tapesSize);
                }
                a[l][q] = -1;
                a[l][r] = -2;

                if (a[l - 1][q] == 0) break;
            }

            while (true)
            {
                bool isReverseMerge;
                vector <Queue*> TapesToMerge;
                for (int i = 0; i < tapesSize; i++)
                {
                    if (i == r) continue;
                    if (d[i] > 0)
                    {
                        d[i]--;
                    }
                    else
                    {
                        TapesToMerge.push_back(Tapes[i]);
                        if (a[l][i] >= 0)
                            isReverseMerge = a[l][i] % 2 == 0 ? true : false;
                        else
                        {
                            int k = l - 1;
                            while (a[k][i] < 0) k--;
                            isReverseMerge = a[k][i] % 2 == 0 ? true : false;
                        }
                    }
                }
                if (TapesToMerge.size() == 0)
                {
                    d[r]++;
                }
                else
                {
                    TapesMerge(TapesToMerge, Tapes[r], isDirectReading, isReverseMerge);
                }
                s++;
                l--;
                a[l][r] = s;
                a[l][q] = -1;
                r = (2 * q - r) % tapesSize;

                if (l == 0) break;
                bool temp = true;
                for (int i = 0; i < tapesSize; i++)
                {
                    if (i == q || i == r) continue;
                    if (a[l][i] != s)
                    {
                        temp = false;
                        break;
                    }
                }
                if (temp)
                {
                    if (a[l - 1][q] != s) break;
                }
                else break;
            }
            if (l == 0) break;
        }
    }
}

bool TestOscillatingSort(int a, vector <Queue*>& Tapes)
{
    Queue* InputData = new Queue();
    for (int i = 0; i < a; i++)
    {
        InputData->Push(new MyObject(rand()));
    }

    int q = OscillatingSort(Tapes, InputData);
    Tapes[q]->PopBack();
    Tapes[q]->PopFront();

    bool isRightOrder = true;
    while (Tapes[q]->length > 0)
    {
        int a = Tapes[q]->PopFront()->key;
        int b = Tapes[q]->PopFront()->key;
        if (b < a || (Tapes[q]->length == 1 && Tapes[q]->PopFront()->key < b))
        {
            isRightOrder = false;
            break;
        }
    }
    return isRightOrder;
}

bool TestCrissCrossOscillatingSort(int a, vector <Queue*>& Tapes)
{
    Queue* InputData = new Queue();
    for (int i = 0; i < a; i++)
    {
        InputData->Push(new MyObject(rand()));
    }

    int q = CrissCrossOscillatingSort(Tapes, InputData);
    Tapes[q]->PopBack();
    Tapes[q]->PopFront();

    bool isRightOrder = true;
    while (Tapes[q]->length > 0)
    {
        int a = Tapes[q]->PopFront()->key;
        int b = Tapes[q]->PopFront()->key;
        if (b < a || (Tapes[q]->length == 1 && Tapes[q]->PopFront()->key < b))
        {
            isRightOrder = false;
            break;
        }
    }
    return isRightOrder;
}