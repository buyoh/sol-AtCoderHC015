use std::{collections::*, io::Read, ops::Sub};

fn take_token<R: std::io::BufRead>(cin: &mut R) -> String {
    cin.bytes()
        .map(|c| c.unwrap() as char)
        .skip_while(|c| c.is_whitespace())
        .take_while(|c| !c.is_whitespace())
        .collect::<String>()
}
#[allow(unused)]
macro_rules! scan {
    ($io:expr => $t:ty) => (take_token(&mut $io).parse::<$t>().unwrap());
    ($io:expr => $t:tt * $n:expr) => ((0..$n).map(|_| scan!($io => $t)).collect::<Vec<_>>());
    ($io:expr => $($t:tt),*) => (($(scan!($io => $t)),*));
    ($io:expr => $($t:tt),* * $n:expr) => ((0..$n).map(|_| ($(scan!($io => $t)),*)).collect::<Vec<_>>());
}

fn main() {
    solve(std::io::stdin().lock())
}

fn calc_score(items: &Vec<(i32, i32)>, selected: &Vec<i32>) -> i32 {
    let mut score = 0;
    for i in selected.iter() {
        score += items[*i as usize].0;
    }
    return score;
}

fn solve<R: std::io::BufRead>(mut cin: R) {
    let (n, capacity) = scan!(cin => i32, i32);
    let items = scan!(cin => i32, i32*n);
    let mut selected = Vec::<i32>::new();
    let mut total_weight = 0 as i32;
    for (i, (_value, weight)) in items.iter().enumerate() {
        if total_weight + weight <= capacity {
            selected.push(i as i32);
            total_weight += weight;
        }
    }
    for i in selected.iter() {
        println!("{}", i);
    }
    eprintln!("{}", calc_score(&items, &selected));
}
