use hex;
use base64;
use std::collections::HashMap;

 //1.1
fn hex_to_base64(hex_string: String) -> String {
    let decoded = hex::decode(hex_string);
    let encoded = base64::encode(&decoded.unwrap());
    return encoded;
}
//1.2
fn fixed_xor(right_side: String, left_side: String) -> String {
    let decoded_right = hex::decode(right_side).unwrap();
    let decoded_left = hex::decode(left_side).unwrap();
    let result = fixed_xor_bytes(&decoded_right, &decoded_left);
    return hex::encode(&result);
}

fn fixed_xor_bytes(right: &Vec<u8>, left: &Vec<u8>) -> Vec<u8> {
    let mut result: Vec<u8> = Vec::new();
    for i in 0..left.len() {
        result.push(right[i] ^ left[i]);
    }
    return result
}

fn xor_vector_against_single_byte(byte_vector: &Vec<u8>, single: u8) -> Vec<u8> {
    let mut result: Vec<u8> = Vec::new();
    for i in 0..byte_vector.len() {
        result.push(byte_vector[i] ^ single);
    }
    return result
}

fn score_sequence(bytes: &Vec<u8>, table: &HashMap<u8, i32>) -> i32 {
    let mut result: i32 = 0;
    for i in 0..bytes.len() {
        let byte_value = table.get(&bytes[i]);
        result = result + byte_value.unwrap_or(&0);
    }
    return result;
}

//1.3
pub fn solve_single_byte_xor_cipher() {
    let message = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    let unhexed_message = hex::decode(message).unwrap();
    let mut test_byte: u8 = 0;
    println!("{:?}", unhexed_message);
    // seeing if etaoin shrdlu *only* might be a shortcut
    // narrator: it was not. I didn't get the right answer until I
    // included spaces
    let mut score_table: HashMap<u8, i32> = HashMap::new();
    // there has got to be a less stupid way to do this
    score_table.insert(b' ', 13);
    score_table.insert(b'e', 12);
    score_table.insert(b'E', 12);
    score_table.insert(b't', 9);
    score_table.insert(b'T', 9);
    score_table.insert(b'a', 8);
    score_table.insert(b'A', 8);
    score_table.insert(b'o', 8);
    score_table.insert(b'O', 8);
    score_table.insert(b'i', 7);
    score_table.insert(b'I', 7);
    score_table.insert(b'n', 7);
    score_table.insert(b'N', 7);
    score_table.insert(b's', 6);
    score_table.insert(b'S', 6);
    score_table.insert(b'r', 6);
    score_table.insert(b'R', 6);
    score_table.insert(b'h', 6);
    score_table.insert(b'H', 6);
    score_table.insert(b'D', 4);
    score_table.insert(b'd', 4);
    score_table.insert(b'l', 4);
    score_table.insert(b'L', 4);
    score_table.insert(b'u', 3);
    score_table.insert(b'U', 3);
    score_table.insert(b'p', 2);
    score_table.insert(b'P', 2);
    score_table.insert(b'b', 1);
    score_table.insert(b'B', 1);
    score_table.insert(b'v', 1);
    score_table.insert(b'V', 1);
    score_table.insert(b'k', 1);
    score_table.insert(b'K', 1);
    score_table.insert(b'x', 1);
    score_table.insert(b'X', 1);
    score_table.insert(b'q', 1);
    score_table.insert(b'Q', 1);

    struct MessageScore {
        message: Vec<u8>,
        score: i32
    }
    let mut best_message = MessageScore {
        message: Vec::new(),
        score: 0
    };
    for i in 0..256 {
        //println!("trying byte {}...", i);
        let xor_message = xor_vector_against_single_byte(&unhexed_message, test_byte);
        let score = score_sequence(&xor_message, &score_table);
        //println!("score: {}, xor_message: {:?}", score, xor_message);
        if score > best_message.score {
            best_message.score = score;
            best_message.message = xor_message;
            let maybe_string = String::from_utf8(best_message.message);
            if maybe_string.is_err() {
                println!("new best score of {} but the message wasn't printable", score)
            } else {
                println!("new best score:{}, message: {}", score, maybe_string.unwrap())
            }
        }
        if i < 255 {   
            test_byte = test_byte + 1;
        }    
    }
    //println!("{}", unhexed_message.len());
}

#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_hex() {
        assert_eq!(hex_to_base64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d".to_string()), 
            "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
    }

    #[test]
    fn test_fixed() {
        assert_eq!(fixed_xor("1c0111001f010100061a024b53535009181c".to_string(), "686974207468652062756c6c277320657965".to_string()), 
            "746865206b696420646f6e277420706c6179");
    }
}