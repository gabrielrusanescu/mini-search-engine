Mini Search Engine (Divide et Impera) A lightweight C-based search engine utility that ranks text documents based on the frequency of a search term. This project was developed as a practical application of the Divide and Conquer algorithmic strategy.

Overview This program processes a list of text files, calculates the "relevance" of each document (defined by the number of occurrences of a specific keyword), and outputs a sorted list of documents starting with the most relevant.

Key Features Divide et Impera Implementation: Uses a recursive approach to process document batches and merge results efficiently.

Relevance Scoring: Ranks results based on exact word matches.

Dynamic Memory Management: Handles document paths and results using dynamic allocation.

Integrated Testing: Includes a suite of assert tests to verify search accuracy and ranking logic.

How It Works The core logic follows the Divide and Conquer paradigm, similar to a Merge Sort:

Divide: The list of documents is recursively split into two halves until individual documents are reached.

Impera (Conquer): For each document, the calculeaza_relevanta function counts how many times the search term appears.

Combine: The interclaseaza_rezultate function merges the results from the left and right sub-problems, keeping the final list sorted by score in descending order.
