"""
 - Richard Buckley 07/22/2023
 - Test Generation for Sorting Algorithms

 Criteria:
    - We must test sorting algorithms with numerous test cases
    - We must apply edge cases (e.g. sorted, reverse sorted, etc.)
    - We must run each type of test numerous times
    - Duplicate elements
    - Partially Sorted
    - Outliers (large range of values)
    - Large Input Size

 Solution:
    - We are going to cover three main categories / rules to follow:
        1. EDGE Cases (edge_cases.py)
            a. Sorted
            b. Reverse Sorted
            c. Partially Sorted
            d. Random Array

        2. Large Input Size (generate_cases.py)
            a. We will run each test with different
                arrays ranging from 100 elements to 1,000 elements.
                (increments of 100)

        3. Reproducible Results (generate_cases.py)
            a. We want to run each test 10 times. This will ensure
                that the conclusions that we draw will be repeatable.

    We are just going to be using python's random library because it does not matter
    how cryptographically secure our random number selection is.
"""
import os.path
import uuid
import random
import pandas as pd
import subprocess
from typing import List, Mapping

#
# Section 1:
# EDGE Cases
#


def sorted_array(n: int, m: int = 1) -> List[int]:
    """
    Generates a sorted array beginning at m and ending at n + m
    :param n: Number of elements in array
    :param m: Starting Number
    :return: sorted list of integers (step of 1)
    """
    return [i for i in range(m, n + m)]


def reverse_sorted_array(n: int, m: int = 1) -> List[int]:
    """
    Generates a reverse sorted array beginning at n + m and ending at m
    :param n: Number of elements in array
    :param m: Starting Number
    :return: sorted List of integers (step of -1)
    """
    return [i for i in range(n + m, m, -1)]


def partially_sorted_array(k: int, n: int, m: int = 1) -> List[int]:
    """
    Generates a partially sorted array with the first k elements sorted
    :param k: Number of elements to have sorted
    :param n: Number of elements in array
    :param m: Starting element in array
    :return: partially sorted list of integers (step of 1)
    """
    return [idx if idx < k else random.randint(m, n + m) for idx in range(m, n + m)]


def random_array(n: int, k: int = 1) -> List[int]:
    """
    Generates an array of random elements of size n, that have a value of k -> n * k,
    where k is a scaler (and is 1 by default)
    :param n: number of elements in random array
    :param k: scaler
    :return: unsorted array of random integers
    """
    return random.sample(range(1, n * k), n)


#
# Section 2:
# Storing Results in CSV
#

# This array will be used for both creating the tests & running the c++ program
ALGORITHMS = ["insertion_sort", "merge_sort", "quick_sort", "radix_sort"]

# This is where the tests are located
TESTS_FOLDER = "./tests"

# This is the path to access the tests


def t(file): return f"{TESTS_FOLDER}/{file}.csv"


# This will be a map representing the csv file structure
times = [f"Elapsed Time (ms - {algorithm})" for algorithm in ALGORITHMS]
test_csv_structure = ["Test", "Input Array", "Output Array"] + times


def create_csv_test(test_name: str):
    """
    Create a csv file for a given test.
    :param test_name: the name of the test to write the file to
    :return: void -> this will just create a csv file
    """
    # Don't overwrite anything!
    if os.path.exists(t(test_name)):
        return

    # Make sure the folder exists
    os.makedirs(os.path.dirname(t(test_name)), exist_ok=True)

    # Create file if it doesn't exist
    with open(t(test_name), "x") as file:
        file.write(','.join(test_csv_structure) + '\n')


def cast_to_cols(lines: List[str]) -> Mapping[str, str]:
    return {key: lines[i] if len(lines) > i else "" for i, key in enumerate(test_csv_structure[1:])}


def write_csv_test(test_name: str, cols: Mapping[str, str]):
    """
    Write a line to a csv test
    :param test_name: the name of the test to record
    :param cols: a map of columns with their corresponding values
    :return: void
    """
    create_csv_test(test_name)

    # Create a new map to ensure that we have only columns that are in the test_csv_structure
    add_map = {key: value for key,
               value in cols.items() if key in test_csv_structure}

    # Open the file, and append
    with open(t(test_name), "a+") as file:
        file.seek(0)

        # This is literally just the test number (e.g. 1, 2, 3, 4, 5, etc.)
        add_map["Test"] = str(sum(1 for _ in file.readlines()))
        file.write(','.join(str(val) for val in add_map.values()) + '\n')


# Now that we have a way to create csvs for the individual tests, we need a way to create csvs
# for the resulting files.
RESULT_FILE = "Results"
result_csv_structure = ["Test Name", *
                        [f"Mean (ms - {algo})" for algo in ALGORITHMS]]


def create_csv_result():
    """
    Create a csv file for the results.
    :return: void -> this will just create a csv file
    """
    # Don't overwrite anything!
    if os.path.exists(t(RESULT_FILE)):
        return

    # Create file if it doesn't exist
    with open(t(RESULT_FILE), "x") as file:
        file.write(','.join(result_csv_structure) + '\n')


def cast_to_results(test_name: str, avg_times: List[int]):
    """
    This just assigns the avg_times to the correct columns
    """
    data = {"Test Name": test_name}

    for i, col in enumerate(result_csv_structure[1:]):
        data[col] = str(avg_times[i])

    return data


def write_csv_result(cols: Mapping[str, str]):
    """
    Write a line to a csv file
    :param cols: a map of the cols
    :return: void -> this just adds to result
    """
    create_csv_result()

    # Create map of required columns
    add_map = {key: cols[key]
               if key in cols else "" for key in result_csv_structure}

    # Open the file and append
    with open(t(RESULT_FILE), "a") as file:
        file.write(','.join(str(val) for val in add_map.values()) + '\n')


#
# Section 3:
# Preparing to Compile / Testing
#

TEMP_FOLDER = "./.temp"


def write_to_temp_file(input_arr: List[int]) -> str:
    """
    This creates a temp file with a random name, and
    returns its path. This file will be stored in the
    ./.temp, and will be deleted on exit of this program
    :param input_arr: the input array
    :return: the temporary filename
    """
    if not os.path.exists(TEMP_FOLDER):
        os.mkdir(TEMP_FOLDER)

    file_name = str(uuid.uuid4())
    file_path = f"{TEMP_FOLDER}/{file_name}"

    with open(file_path, "w") as file:
        file.write(' '.join([str(num) for num in input_arr]) + '\n')

    return file_path


def clear_temp():
    """
    Deletes the temp folder
    :return:
    """
    if os.path.exists(TEMP_FOLDER) and len(os.listdir(TEMP_FOLDER)) == 0:
        os.rmdir(TEMP_FOLDER)


def read_temp_file(file_path: str) -> List[str]:
    """
    Reads the data from a temp file, and returns the lines
    in a List of strings. This also deletes the temp file
    :param file_path: the path to the file
    :return: lines
    """
    lines: List[str]

    # Read lines
    with open(file_path, "r") as file:
        file.seek(0)

        # Make sure that we aren't copying a reference
        lines = [line.strip() for line in file.readlines()]

    # delete the file now that it has been read
    os.remove(file_path)

    # check if folder is empty... delete?
    if len(os.listdir(TEMP_FOLDER)) == 0:
        clear_temp()

    # return lines
    return lines


"""
 - As you see these functions, you may realize the way that I am deciding to do this...
 - python creates temp file, gives path to cpp program
 - cpp program runs, adds results to temp file
 - python sees temp file. python deletes temp file
 - ...
 - but why python? Well this is kind of future-proofing statistics. It is extremely easy
 - to do complex data science things in python...so why not use it. 
 - also if we ever decide to run larger scale tests, this would allow us to generate the
 - tests that we want to run at one time, and then run them at a later time (say you are 
 - renting a machine to run the tests, etc)
 
 Here is the structure of the `./.temp/<random_uuid>`
    --- FILE START ---
    {INPUT ARR}
    {SORTED ARR}            <-- cpp
    {TIME FOR INSERTION}    <-- cpp
    {TIME FOR MERGE}        <-- cpp
    {TIME FOR QUICK}        <-- cpp
    {TIME FOR RADIX         <-- cpp
    --- FILE END ---
"""


COMPILED_NAME = "./timeit"
COMPILED_EXISTS = os.path.exists(COMPILED_NAME)


def compile_file():
    """
    Compiles the cpp file if it is not yet compiled.
    :return:
    """
    if not COMPILED_EXISTS:
        subprocess.call(
            ["g++", "timeit.cpp", "sorts.cpp", "-o", COMPILED_NAME])


def test_algorithm(test_name: str, input_arr: List[int], n: int = 1):
    """
    Tests the algorithm by running the cpp file, and gathering the results
    :param test_name: the name of the test to record
    :param input_arr: the input to run the test with
    :param n: the number of times to run the cpp file
    :return: void
    """
    # compile the .cpp files
    compile_file()

    # loop this n times
    while n >= 1:
        # Generate temp file
        temp_file = write_to_temp_file(input_arr)

        # Run the c++ file
        subprocess.call(["./timeit", temp_file])

        # Gather & Write results
        lines = read_temp_file(temp_file)

        # Write Results
        write_csv_test(test_name, cast_to_cols(lines))

        # Decrement
        n -= 1


def clean_up():
    """
    Delete Temp & Compiled file
    :return:
    """
    clear_temp()
    if os.path.exists(COMPILED_NAME):
        os.remove(COMPILED_NAME)


def perform_tests():
    # random helper
    def rand(x): return [random.randint(100, 10_000) for _ in range(x)]

    # We want to run m tests for each of the edge cases ...
    m = 100
    rep = 10

    # Case 1 Sorted arrays
    for i in range(m):
        n, k = rand(2)
        print(f"Running {i}/{m}...")
        test_algorithm("Correctly-Sorted", sorted_array(n, k), rep)

    # Case 2 Reverse Sorted Array
    for i in range(m):
        n, k = rand(2)
        print(f"Running {i}/{m}...")
        test_algorithm("Reverse-Sorted", reverse_sorted_array(n, k), rep)

    # Case 3 Partially Sorted Array
    for i in range(m):
        a, b, c = rand(3)
        print(f"Running {i}/{m}...")
        test_algorithm("Partial-Sorted", partially_sorted_array(a, b, c), rep)

    # Case 4 Random Arrays
    for i in range(m):
        n, k = rand(2)
        print(f"Running {i}/{m}...")
        test_algorithm("Random-Sorted", random_array(n, k), rep)

    # Case 5 Outliers
    for i in range(m):
        n, k = rand(2)
        print(f"Running {i}/{m}...")
        test_algorithm("Random-Outliers", random_array(n, k) +
                       [random.randint(100_000, 1_000_000)], rep)


def analyze_test():
    if os.path.exists(t(RESULT_FILE)):
        os.remove(t(RESULT_FILE))

    tests = ["Correctly-Sorted", "Partial-Sorted",
             "Random-Sorted", "Reverse-Sorted", "Random-Outliers"]

    for test in tests:
        df = pd.read_csv(t(test))
        means = []

        for col in test_csv_structure[3:]:
            means.append(df[col].mean())

        write_csv_result(cast_to_results(test, means))

    transpose_csv()


def transpose_csv():
    df = pd.read_csv(t(RESULT_FILE))
    df.transpose()
    df.to_csv(t(RESULT_FILE), index_label="Index")


if __name__ == "__main__":

    perform_tests()
    analyze_test()

    clean_up()
