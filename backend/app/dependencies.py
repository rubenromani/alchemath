from typing import Generator
import logging

logger = logging.getLogger(__name__)

def get_logger() -> logging.Logger:
    """Dependency to get logger instance."""
    return logger

# Future dependencies for database, authentication, etc.
# def get_db() -> Generator:
#     """Database dependency."""
#     pass
# 
# def get_current_user():
#     """Authentication dependency."""
#     pass