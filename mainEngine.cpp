#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;


//Edge
// A ----4km----> B
// Represents one road from the current node to another node

struct Edge {
    int destination;    // Where does this edge lead?
    double distance;    // Distance of this road
};


// PATH RESULT
struct PathResult {

    double distance;

    vector<int> path;
};



// GRAPH
class Graph {

private:

    // adjacencyList[node] contains
    // all roads connected to that node.
    vector<vector<Edge>> adjacencyList;


public:

    // Constructor
    Graph(int noOfLocations);

    // Add a road
    void addRoad(int from,int to,double distance);

    // Run Dijkstra
    PathResult dijkstra(int source,int destination);
};


// GRAPH CONSTRUCTOR
Graph::Graph(int noOfLocations) {

    adjacencyList.resize(noOfLocations);
}

void Graph::addRoad(int from,int to,double distance) {

    // from -> to
    adjacencyList[from].push_back({
        to,
        distance
    });

    // to -> from
    // Our city roads are undirected.
    adjacencyList[to].push_back({from,distance});
}



// DIJKSTRA
PathResult Graph::dijkstra(
    int source,
    int destination
) {

    // distance[node] stores the shortest
    // distance currently known from source
    vector<double> distance(adjacencyList.size(),numeric_limits<double>::infinity());

    // parent[node] stores the previous node
    // in the shortest path.
    vector<int> parent(adjacencyList.size(),-1);


    // Min-heap.
    // Stores:
    // (distance, node)
    // Smallest distance comes first.
    priority_queue<pair<double, int>,vector<pair<double, int>>,greater<pair<double, int>>> pq;

    distance[source] = 0;

    // Start Dijkstra from source.

    pq.push({0,source});

    while (!pq.empty()) {
        auto [currentDistance, currentNode] =pq.top();
        pq.pop();
        if (currentDistance >distance[currentNode]) {
            continue;
        }


        // Visit all neighbours of current node.
        for (const Edge& edge : adjacencyList[currentNode]) {

            // Distance to neighbour
            // through current node.

            double newDistance =currentDistance+edge.distance;
            if (newDistance < distance[edge.destination]) {

                // Update shortest distance.
                distance[edge.destination]=newDistance;

                // Remember previous node.
                parent[edge.destination] =currentNode;
                // Add updated route
                // to priority queue.

                pq.push({newDistance,edge.destination});
            }
        }
    }


    // ========================================================
    // RECONSTRUCT SHORTEST PATH
    // ========================================================

    vector<int> path;

    int current = destination;


    // We start at destination and move
    // backwards using the parent array.

    while (current != -1) {

        path.push_back(current);

        current = parent[current];
    }


    // Path was constructed backwards,
    // so reverse it.

    reverse(path.begin(),path.end());


    // Return both distance and path.

    return {distance[destination],path};
}

class Person {

protected:

    int id;

    string name;

    int currentLocation;


public:

    // Constructor

    Person(int id,string name,int currentLocation): id(id),name(name),currentLocation(currentLocation){}
    // Get ID
    int getId() const {

        return id;
    }


    // Get name

    string getName() const {

        return name;
    }


    // Get current location

    int getCurrentLocation() const {

        return currentLocation;
    }


    // Pure virtual function.
    //
    // Makes Person an abstract class.

    virtual void displayInfo() const = 0;


    // Virtual destructor.

    virtual ~Person() = default;
};


class Customer : public Person {

public:

    Customer(int id,string name,int currentLocation): Person(id,name,currentLocation){}

    void displayInfo() const override {
        cout << "Customer ID: "<< id<< endl;
        cout << "Name: "<< name<< endl;

        cout << "Location: "<< currentLocation<< endl;
    }
};

class Rider : public Person {
private:
    bool available;

public:
    Rider(int id, string name, int currentLocation, bool available)
        : Person(id, name, currentLocation),
          available(available) {}

    bool isAvailable() const {
        return available;
    }

    void displayInfo() const override {
        cout << "Rider ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Location: " << currentLocation << endl;
        cout << "Available: "
             << (available ? "Yes" : "No")
             << endl;
    }
};


class RiderManager {
private:
    vector<Rider> riders;

public:
    void addRider(const Rider& rider);

    Rider* findNearestRider(
        const Customer& customer,
        Graph& city
    );
};


void RiderManager::addRider(const Rider& rider) {
    riders.push_back(rider);
}


Rider* RiderManager::findNearestRider(
    const Customer& customer,
    Graph& city
) {
    Rider* bestRider = nullptr;

    double minimumDistance =
        numeric_limits<double>::infinity();

    for (Rider& rider : riders) {

        // Skip riders who are currently unavailable.
        if (!rider.isAvailable()) {
            continue;
        }

        // Find the shortest route from the rider
        // to the customer's current location.
        PathResult result = city.dijkstra(
            rider.getCurrentLocation(),
            customer.getCurrentLocation()
        );

        // Keep the rider with the smallest distance.
        if (result.distance < minimumDistance) {
            minimumDistance = result.distance;
            bestRider = &rider;
        }
    }

    return bestRider;
}


int main() {
    // Create a city with 5 locations.
    Graph city(5);

    // Add roads between locations.
    city.addRoad(0, 1, 4);
    city.addRoad(0, 2, 2);
    city.addRoad(1, 2, 3);
    city.addRoad(1, 3, 5);
    city.addRoad(2, 4, 6);
    city.addRoad(3, 4, 2);

    // Create a customer at location 0.
    Customer customer(
        101,
        "Abhishek",
        0
    );

    // Create the rider manager.
    RiderManager riderManager;

    // Add riders.
    riderManager.addRider(
        Rider(201, "Rahul", 0, true)
    );

    riderManager.addRider(
        Rider(202, "Amit", 4, true)
    );

    riderManager.addRider(
        Rider(203, "Raj", 2, false)
    );

    // Find the nearest available rider.
    Rider* bestRider =
        riderManager.findNearestRider(
            customer,
            city
        );

    // Display the result.
    if (bestRider == nullptr) {
        cout << "No rider available." << endl;
    } else {
        cout << "Nearest Rider: "
             << bestRider->getId()
             << endl;

        cout << "Rider Name: "
             << bestRider->getName()
             << endl;

        cout << "Rider Location: "
             << bestRider->getCurrentLocation()
             << endl;
    }

    return 0;
}