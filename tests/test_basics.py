import os

from cgshop2024_pyutils import (
    BadSolutionFile,
    InstanceDatabase,
    InvalidSolution,
    read_solution,
    verify,
)


def get_path(rel_path):
    TEST_DIR = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(TEST_DIR, rel_path)


def get_db():
    path = get_path("./assets/cgshop2024_benchmark.zip")
    if not os.path.exists(path):
        # download the benchmark
        import urllib.request

        url = "https://cgshop.ibr.cs.tu-bs.de/worker_uploads_static/cgshop2024_benchmark.zip"
        urllib.request.urlretrieve(url, path)

    return InstanceDatabase(path)


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
    assert rverify(instance, solution) is True


def test_overlap():
    db = get_db()
    solution = _read_solution("./assets/test_overlap.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_bounds():
    db = get_db()
    solution = _read_solution("./assets/test_bounds.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_outside():
    db = get_db()
    solution = _read_solution("./assets/test_outside.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_multi():
    db = get_db()
    solution = _read_solution("./assets/test_multi.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_negindex():
    db = get_db()
    solution = _read_solution("./assets/test_negindex.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_highindex():
    db = get_db()
    solution = _read_solution("./assets/test_highindex.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_same_overlap():
    db = get_db()
    solution = _read_solution("./assets/test_same_overlap.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is False


def test_same_used():
    db = get_db()
    solution = _read_solution("./assets/test_same_used.json")
    instance = db[solution["instance_name"]]
    assert rverify(instance, solution) is True
