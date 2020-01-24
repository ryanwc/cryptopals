use hex;
use base64;

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
    let mut result: Vec<u8> = Vec::new();
    for i in 0..decoded_left.len() {
        result.push(decoded_right[i] ^ decoded_left[i]);
    }
    return hex::encode(&result);
}

pub fn solve_single_byte_xor_cipher() {
    let message = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    let unhexed_message = hex::decode(message).unwrap();
    let mut test_byte: u8 = 0;
    for i in 0..u8::max_value(){
        println!("{}", test_byte);
        test_byte = test_byte + 1;
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