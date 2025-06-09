#!/usr/bin/bash


languages=("C" "C++" "Python" "Java")
extensions=("c" "cpp" "py" "java")
names=()
students=()
line_count=()
comment_count=()
function_count=()
file_type=()
Matched=()
Unmatched=()

submission_folder=$1
target_folder=$2
test_folder=$3
answer_folder=$4


if [[ $# -lt 4 ]];then
    echo "Wrong number of arguments"
    echo "Arguments pattern: "
    echo "  1.Path to submission folder"
    echo "  1.Path to targets folder"
    echo "  1.Path to tests folder"
    echo "  1.Path to answers folder"
    echo "Optional Arguments: "
    echo "  -v -noexecute -nolc -nocc -nofc"
    kill -INT $$
fi

if [[ ! -d "$submission_folder" ]];then
    echo "Submission folder not found"
    kill -INT $$
elif [[ ! -d "$test_folder" ]];then
    echo "Test folder not found"
    kill -INT $$
elif [[ ! -d "$answer_folder" ]];then
    echo "Answer folder not found"
    kill -INT $$
fi

v=0
noexecute=0
nolc=0
nocc=0
nofc=0
shift 4

while [ $# -gt 0 ]; do
    case "$1" in
        -v)
            v=1
            ;;
        -noexecute)
            noexecute=1
            ;;
        -nolc)
            nolc=1
            ;;
        -nocc)
            nocc=1
            ;;
        -nofc)
            nofc=1
            ;;
    esac
    shift
done

mkdir -p "$target_folder"
mkdir -p "$target_folder"/temporary
for language in "${languages[@]}"
do
    mkdir -p "$target_folder"/"$language"
done

for zip in "$submission_folder"/*
do
    unzip -q "$zip" -d "$target_folder"/temporary
done

get_code() {
    if [[ -f "$1" ]];then
        extension=`basename "$1" | awk -F '.' '{print $NF}'`
        if [[ "$extension" == "c" || "$extension" == "cpp" || "$extension" == "py" || "$extension" == "java" ]];then
            echo "$1"
        fi
    elif [[ -d "$1" ]];then
        for i in "$1"/*
        do
            get_code "$i"
        done
    fi
}

for file in "$target_folder"/temporary/*
do
    student_id=`basename "$file" | awk -F '_' '{print $NF}'`
    name=`basename "$file" | awk -F '_' '{print $1}'`
    names+=("$name")
    students+=("$student_id")
    if [[ v -eq 1 ]];then
        echo "Organizing files of $student_id"
    fi
    main_file=`get_code "$file"`
    line_count+=("$(grep -c '' "$main_file")")
    main_file_extension=`basename "$main_file" | awk -F '.' '{print$NF}'`
    if [[ "$main_file_extension" == "c" ]];then
        comment_count+=("$(grep -c '//' "$main_file")")
        function_count+=("$(grep -cE "( )*[A-Za-z0-9]+( )+[A-Za-z0-9_]+( )*\(" "$main_file")")
        file_type+=("C")
        mkdir -p "$target_folder"/C/"$student_id"
        mv "$main_file" "$target_folder"/C/"$student_id"/main.c

        if [[ noexecute -eq 0 ]];then
            if [[ v -eq 1 ]];then
                echo "Executing files of $student_id"
            fi
            c_folder="$target_folder"/C/"$student_id"
            gcc "$c_folder"/main.c -o "$c_folder"/main.out
            declare -i out_txt=1
            for test in "$test_folder"/*
            do
                "$c_folder"/main.out < "$test" > "$c_folder"/out"$out_txt".txt
                out_txt+=1
            done
            declare -i matched_output=0
            declare -i unmatched_output=0
            declare -i checked=1
            for ans in "$answer_folder"/* 
            do
                if diff "$ans" "$c_folder"/out"$checked".txt > /dev/null;then
                    ((matched_output++))
                else
                    ((unmatched_output++))
                fi
                checked+=1
            done
            Matched+=($matched_output)
            Unmatched+=($unmatched_output)
        fi

    elif [[ "$main_file_extension" == "cpp" ]];then
        comment_count+=("$(grep -c '//' "$main_file")")
        function_count+=("$(grep -cE "^( )*[A-Za-z0-9]+( )+[A-Za-z0-9_]+( )*\(" "$main_file")")
        file_type+=("C++")
        mkdir -p "$target_folder"/C++/"$student_id"
        mv "$main_file" "$target_folder"/C++/"$student_id"/main.cpp

        if [[ noexecute -eq 0 ]];then
            if [[ v -eq 1 ]];then
                echo "Executing files of $student_id"
            fi
            cpp_folder="$target_folder"/C++/"$student_id"
            g++ "$cpp_folder"/main.cpp -o "$cpp_folder"/main.out
            declare -i out_txt=1
            for test in "$test_folder"/*
            do
                "$cpp_folder"/main.out < "$test" > "$cpp_folder"/out"$out_txt".txt
                out_txt+=1
            done
            declare -i matched_output=0
            declare -i unmatched_output=0
            declare -i checked=1
            for ans in "$answer_folder"/* 
            do
                if diff "$ans" "$cpp_folder"/out"$checked".txt > /dev/null;then
                    ((matched_output++))
                else
                    ((unmatched_output++))
                fi
                checked+=1
            done
            Matched+=($matched_output)
            Unmatched+=($unmatched_output)
        fi

    elif [[ "$main_file_extension" == "py" ]];then
        comment_count+=("$(grep -c '#' "$main_file")")
        function_count+=("$(grep -cE "^( )*def" "$main_file")")
        file_type+=("Python")
        mkdir -p "$target_folder"/Python/"$student_id"
        mv "$main_file" "$target_folder"/Python/"$student_id"/main.py

        if [[ noexecute -eq 0 ]];then
            if [[ v -eq 1 ]];then
                echo "Executing files of $student_id"
            fi
            py_folder="$target_folder"/Python/"$student_id"
            declare -i out_txt=1
            for test in "$test_folder"/*
            do
                chmod u+x "$py_folder"/main.py
                python3 "$py_folder"/main.py < "$test" > "$py_folder"/out"$out_txt".txt
                out_txt+=1
            done
            declare -i matched_output=0
            declare -i unmatched_output=0
            declare -i checked=1
            for ans in "$answer_folder"/* 
            do
                if diff "$ans" "$py_folder"/out"$checked".txt > /dev/null;then
                    ((matched_output++))
                else
                    ((unmatched_output++))
                fi
                checked+=1
            done
            Matched+=($matched_output)
            Unmatched+=($unmatched_output)
        fi

    elif [[ "$main_file_extension" == "java" ]];then
        comment_count+=("$(grep -c '//' "$main_file")")
        function_count+=("$(grep -cE "^( )*[A-Za-z0-9]*( )+[A-Za-z0-9]*( )+[A-Za-z0-9]+( )+[A-Za-z0-9_]+( )*\(" "$main_file")")
        file_type+=("Java")
        mkdir -p "$target_folder"/Java/"$student_id"
        mv "$main_file" "$target_folder"/Java/"$student_id"/Main.java

        if [[ noexecute -eq 0 ]];then
            if [[ v -eq 1 ]];then
                echo "Executing files of $student_id"
            fi
            java_folder="$target_folder"/Java/"$student_id"
            javac "$java_folder"/Main.java
            declare -i out_txt=1
            for test in "$test_folder"/*
            do
                java -cp "$java_folder" Main < "$test" > "$java_folder"/out"$out_txt".txt
                out_txt+=1
            done
            declare -i matched_output=0
            declare -i unmatched_output=0
            declare -i checked=1
            for ans in "$answer_folder"/* 
            do
                if diff "$ans" "$java_folder"/out"$checked".txt > /dev/null;then
                    ((matched_output++))
                else
                    ((unmatched_output++))
                fi
                checked+=1
            done
            Matched+=($matched_output)
            Unmatched+=($unmatched_output)
        fi

    fi
done

if [[ v -eq 1 ]];then
    echo "All submissions processed successfully"
fi

rm -r "$target_folder"/temporary

{
    printf "student_id,student_name,language"
    [ "$noexecute" -eq 0 ] && printf ",matched,not_matched"
    [ "$nolc" -eq 0 ] && printf ",line_count"
    [ "$nocc" -eq 0 ] && printf ",comment_count"
    [ "$nofc" -eq 0 ] && printf ",function_count"
    printf "\n"

    for i in "${!students[@]}"
    do
        printf "${students[$i]}","${names[$i]}","${file_type[$i]}"
        [ "$noexecute" -eq 0 ] && printf ,"${Matched[$i]}","${Unmatched[$i]}"
        [ "$nolc" -eq 0 ] && printf ,"${line_count[$i]}"
        [ "$nocc" -eq 0 ] && printf ,"${comment_count[$i]}"
        [ "$nofc" -eq 0 ] && printf ,"${function_count[$i]}"
        printf "\n"
    done

} > "$target_folder"/result.csv