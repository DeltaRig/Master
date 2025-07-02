def files_are_equal(file1, file2):
    with open(file1, 'r', encoding='utf-8') as f1, open(file2, 'r', encoding='utf-8') as f2:
        lines1 = f1.readlines()
        lines2 = f2.readlines()
        return lines1 == lines2

# Example usage:
file_a = 'dtw_result_dynamic100.csv'
file_b = 'dtw_result_guided100.csv'

if files_are_equal(file_a, file_b):
    print("The files are equal.")
else:
    print("The files are different.")
