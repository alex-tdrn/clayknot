pub use std::any::TypeId;
pub use std::time::Instant;

pub trait Port {
    fn type_id(&self) -> TypeId;

    fn timestamp(&self) -> Instant;
}
