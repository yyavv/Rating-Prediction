#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

struct Data
{
    double item;
    double rating;
};

void swap(Data* a, Data* b)
{

    Data temp = *a;

    *a = *b;

    *b = temp;
}

void heapify(vector<Data>& v, int N, int i)
{

    // Initialize largest as root
    int largest = i;

    // left = 2*i + 1
    int l = 2 * i + 1;

    // right = 2*i + 2
    int r = 2 * i + 2;

    // If left child is larger than root
    if (l < N && v[l].item > v[largest].item)
        largest = l;

    // If right child is larger than largest
    // so far
    if (r < N && v[r].item > v[largest].item)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(v[i], v[largest]);

        // Recursively heapify the affected
        // sub-tree
        heapify(v, N, largest);
    }
}

void heapSort(vector<Data>& v, int N)
{

    // Build heap (rearrange array)
    for (int i = v.size() / 2 - 1; i >= 0; i--)
        heapify(v, N, i);

    // One by one extract an element
    // from heap
    for (int i = N - 1; i > 0; i--) {

        // Move current root to end
        swap(v[0], v[i]);

        // call max heapify on the reduced heap
        heapify(v, i, 0);
    }
}

void mostFrequentUser(unordered_map<int, vector<Data>> map)
{
    cout << "Top users who have the most ratings:" << endl;
    for (int i = 0; i < 10; i++)
    {
        int max_count = 0, res = -1;

        for (auto& m : map)
        {
            if (max_count < m.second.size())
            {
                res = m.first;
                max_count = m.second.size();
            }
        }
        cout << "User: " << res << "\tRatings: " << max_count << endl;
        map.erase(res);
    }
}

void mostFrequentItem(unordered_map<int, int> map)
{
    cout << "Top movies which have the most ratings:" << endl;
    for (int i = 0; i < 10; i++)
    {
        int max_count = 0, res = -1;

        for (auto& m : map)
        {
            if (max_count < m.second)
            {
                res = m.first;
                max_count = m.second;
            }
        }
        cout << "Movie: " << res << "\tRatings: " << max_count << endl;
        map.erase(res);
    }
}

double CosineSimilarity(const vector<Data> x, const vector<Data> y, const int item, double& rating)
{
    double lengthX = 0, lengthY = 0;
    double sum = 0;

    rating = 0;

    for (int i = 0; i < x.size(); i++)
    {
        lengthX += pow(x[i].rating, 2);

        for (int j = 0; j < y.size(); j++)
        {
            if (i == 0)
            {
                lengthY += pow(y[j].rating, 2);

                if (y[j].item == item)
                    rating = y[j].rating;
            }

            if (x[i].item < y[j].item)
                continue;

            else if (x[i].item == y[j].item)
                sum += (x[i].rating * y[j].rating);
        }

        if (rating == 0)
            break;
    }

    if (rating != 0)
        return (sum) / (sqrt(lengthX * lengthY));

    else return 0;
}

double best(const int id, const int item, unordered_map<int, vector<Data>> hashmap)
{

    /*int count[5] = { 0 };
    double guessed_rating[5] = { 0 };*/
    double rating = 0;
    vector<Data> v;

    for (auto const& pair1 : hashmap)
    {
        double OtherUserRating;
        if (pair1.first == id)
            continue;
        double similarity = CosineSimilarity(hashmap[id], pair1.second, item, OtherUserRating);

        if (similarity > 0.05)
        {
            ////cout << pair1.first << "\t" << similarity << "\tItem: " << item << "\tRating:" << OtherUserRating << endl;
            v.push_back({ similarity, OtherUserRating });
        }
    }

    int size = v.size();
    heapSort(v, size);

    if (size >= 25)
    {
        for (int i = 0; i < 25; i++)
        {
            rating += v[size - 1 - i].rating;
        }
        rating /= 25;
    }

    else if (size < 25)
    {
        for (int i = 0; i < size; i++)
        {
            rating += v[i].rating;
        }
        rating /= size;
    }

    else
    {
        cout << "hata!\tuser: " << id << "\tıtem: " << item << endl;
    }	//cout << count << endl;
    return rating;
}

void printVector(const int id, vector<Data> x)
{
    cout << "User: " << id << "\tVector Size: " << x.size() << endl;
    for (int i = 0; i < x.size(); i++)
        cout << "Item: " << x[i].item << "\t" << "Rating: " << x[i].rating << endl;
    cout << endl << endl;
}

void print_map(unordered_map<int, vector<Data>> m)
{
    for (auto const& pair1 : m)
    {
        printVector(pair1.first, pair1.second);
    }
}

int main()
{
    //    it keeps the current time
    clock_t beginning = clock(), end;

    //    it stores all users and their rated movies
    unordered_map<int, vector<Data>> mymap2;

    //    it takes movie as key and it stores how many times that movie rated`
    unordered_map<int, int> items;
    string user, movie, rating;
    string path = "D:\\III. Semester\\CMP2003\\term project\\SecondTry\\train.csv";
    ifstream file(path);

    for (int i = 0; i < 1048576; i++)
    {

        getline(file, user, ',');
        getline(file, movie, ',');
        getline(file, rating);

        if (i == 0)
            continue;

        Data x;
        x.item = stoi(movie);
        x.rating = stod(rating);

        mymap2[stoi(user)].push_back(x);

        items[x.item]++;

    }
    file.close();

    mostFrequentUser(mymap2);
    cout << endl;
    mostFrequentItem(items);

    for (auto& m : mymap2)
    {
        heapSort(m.second, m.second.size());
    }

    path = "D:\\III. Semester\\CMP2003\\term project\\SecondTry\\test.csv";
    string number;
    ifstream file2(path);

    ofstream submission;
    submission.open(("D:\\III. Semester\\CMP2003\\term project\\SecondTry\\try.csv"));

    for (int i = 0; i < 5001; i++)
    {
        getline(file2, number, ',');
        getline(file2, user, ',');
        getline(file2, movie);

        if (i == 0)
        {
            submission << "ID" << "," << "Predicted" << endl;
            continue;
        }

        double rating = best(stoi(user), stoi(movie), mymap2);

        submission << i - 1 << "," << rating << endl;

        cout << (double)i * 100 / 5000 << endl;
    }

    end = clock();
    cout << (double)(end - beginning) / CLOCKS_PER_SEC << " seconds" << endl;
}