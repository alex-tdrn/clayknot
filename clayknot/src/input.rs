pub use crate::port::*;

pub trait Input: Port {}

pub struct InputOf<T: 'static> {
    timestamp: Instant,
}

impl<T> InputOf<T> {
    pub fn new() -> Self {
        Self {
            timestamp: std::time::Instant::now(),
        }
    }
}
