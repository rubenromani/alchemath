from pydantic_settings import BaseSettings
from typing import List

class Settings(BaseSettings):
    PROJECT_NAME: str = "Commodity Spread Analysis API"
    VERSION: str = "1.0.0"
    API_V1_STR: str = "/api/v1"
    
    # CORS settings
    BACKEND_CORS_ORIGINS: List[str] = [
        "http://localhost:3000",
        "http://localhost:8080",
        "http://localhost:5173",  # Vite default
        "https://lovable.dev"     # Your current frontend
    ]
    
    # Future database settings
    DATABASE_URL: str = "postgresql://user:password@localhost/commodity_analysis"
    
    # Future C++ engine settings
    CPP_ENGINE_HOST: str = "localhost"
    CPP_ENGINE_PORT: int = 9090
    
    # Logging
    LOG_LEVEL: str = "INFO"
    
    class Config:
        env_file = ".env"

settings = Settings()
