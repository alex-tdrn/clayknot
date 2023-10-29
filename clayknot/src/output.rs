pub trait Output {}

pub struct OutputOf<T> {
    value: T,
}

impl<T> Output for OutputOf<T> {}
