use std::any::{Any, TypeId};
use std::option::Option;
use std::time::Instant;

pub trait Output {
    fn self_any(&self) -> &dyn Any;

    fn self_any_mut(&mut self) -> &mut dyn Any;

    fn value_any(&self) -> Option<&dyn Any>;

    fn type_id(&self) -> TypeId;

    fn timestamp(&self) -> Instant;
}

pub struct OutputOf<T: 'static> {
    name: String,
    value: Option<T>,
    timestamp: Instant,
}

impl<T> OutputOf<T> {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            value: None,
            timestamp: std::time::Instant::now(),
        }
    }

    pub fn set(&mut self, value: T) {
        self.value = Some(value);
        self.timestamp = std::time::Instant::now();
    }

    pub fn get(&self) -> Option<&T> {
        self.value.as_ref()
    }
}

impl<T> Output for OutputOf<T> {
    fn self_any(&self) -> &dyn Any {
        self
    }

    fn self_any_mut(&mut self) -> &mut dyn Any {
        self
    }

    fn value_any(&self) -> Option<&dyn Any> {
        match self.value.as_ref() {
            Some(v) => Some(v as &dyn Any),
            None => None,
        }
    }

    fn type_id(&self) -> TypeId {
        TypeId::of::<T>()
    }

    fn timestamp(&self) -> Instant {
        self.timestamp
    }
}
