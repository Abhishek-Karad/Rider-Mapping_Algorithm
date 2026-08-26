# Rider Mapping & Route Planning Engine

A backend-focused **C++ project** that simulates the core rider-matching and route-planning logic of a ride-hailing system.

The city is represented as a **weighted graph**, with locations as nodes and roads as weighted edges. The system uses **Dijkstra's algorithm** to find shortest routes and matches customers with the nearest available rider based on road-network distance.

## Features

- Weighted city graph using adjacency lists
- Dijkstra's shortest-path algorithm
- Shortest distance and path reconstruction
- `Person` base class with `Customer` and `Rider` subclasses
- Rider availability management
- Nearest available rider selection
- Priority queue based shortest-path computation

## Tech & Concepts

**C++ • STL • OOP • Graphs • Dijkstra • Priority Queue • Greedy Selection**

## Current Flow

```text
Customer
   ↓
Check Available Riders
   ↓
Calculate Rider → Customer Distance
   ↓
Dijkstra Shortest Path
   ↓
Compare Distances
   ↓
Select Nearest Rider
Planned
Single-source Dijkstra optimization
Ride request & destination handling
Pickup and trip route planning
Fare calculation
Ride lifecycle management
Algorithm performance benchmarking

```
Author
Abhishek Karad
