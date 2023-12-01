use std::collections::HashMap;
use std::fs::read_to_string;


fn read_lines(filename: &str) -> Vec<String> {
    read_to_string(filename) 
        .unwrap()  
        .lines()  
        .map(String::from)  
        .collect()  
}

fn get_number(s : String, lookup : &HashMap<&str, usize>) -> usize {

    let mut ans : usize = 0;

    let mut prev_value : Option<usize> = None;

    for c in 0..s.len() {
        for (key, value) in lookup.iter() {
            if c + key.len() <= s.len() && key[..] == s[c..(c + key.len())] {
                if prev_value.is_none() {
                    ans += 10*value;
                }
                prev_value = Some(*value);
            }
        }
    }

    ans += prev_value.unwrap();

    ans
}

fn main() {

    let lookup = HashMap::from([
        ("one", 1),
        ("two", 2),
        ("three", 3),
        ("four", 4),
        ("five", 5),
        ("six", 6),
        ("seven", 7),
        ("eight", 8),
        ("nine", 9),
        ("1", 1),
        ("2", 2),
        ("3", 3),
        ("4", 4),
        ("5", 5),
        ("6", 6),
        ("7", 7),
        ("8", 8),
        ("9", 9)
    ]);

    let input_filename = "../../inputs/day1.txt";

    let mut ans = 0;

    for line in read_lines(input_filename) {
        ans += get_number(line, &lookup)
    }


    println!("{}", ans);

    assert_eq!(get_number(String::from("one"), &lookup), 11);
    assert_eq!(get_number(String::from("one123"), &lookup), 13);
    assert_eq!(get_number(String::from("oneight"), &lookup), 18);
    assert_eq!(get_number(String::from("oneeight"), &lookup), 18);
    assert_eq!(get_number(String::from("9"), &lookup), 99);
    assert_eq!(get_number(String::from("9234567889"), &lookup), 99);
}
