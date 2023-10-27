from cgshop_pyutils24 import verify, InstanceDatabase, InvalidSolution, BadSolutionFile, read_solution
import os

def get_path(rel_path):
    TEST_DIR = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(TEST_DIR, rel_path)

def get_db():
    return InstanceDatabase(get_path("./assets/cgshop2024_benchmark.zip"))

def _read_solution(path):
    return read_solution(get_path(path))

def rverify(instance, solution):
    try:
        value = verify(instance, solution)
    except InvalidSolution as inv_sol:
        print("Solution is invalid:", inv_sol)
        return False
    except BadSolutionFile as bad_sol:
        print("Solution file is invalid:", bad_sol)
        return False
    else:
        print("Solution is valid and has value", value)
        return True

def test_border():
    db = get_db()
    solution = _read_solution("./assets/test_border.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == True)

def test_overlap():
    db = get_db()
    solution = _read_solution("./assets/test_overlap.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_bounds():
    db = get_db()
    solution = _read_solution("./assets/test_bounds.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_multi():
    db = get_db()
    solution = _read_solution("./assets/test_multi.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_negindex():
    db = get_db()
    solution = _read_solution("./assets/test_negindex.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_highindex():
    db = get_db()
    solution = _read_solution("./assets/test_highindex.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_same_overlap():
    db = get_db()
    solution = _read_solution("./assets/test_same_overlap.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == False)

def test_same_used():
    db = get_db()
    solution = _read_solution("./assets/test_same_used.json")
    instance = db[solution["instance_name"]]
    assert(rverify(instance, solution) == True)