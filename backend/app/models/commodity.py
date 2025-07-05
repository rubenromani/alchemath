from pydantic import BaseModel, Field
from typing import List

class Commodity(BaseModel):
    symbol: str = Field(..., description="Commodity trading symbol (e.g., 'ZC' for Corn)")
    name: str = Field(..., description="Full commodity name")
    available_months: List[str] = Field(..., description="Available contract months")
    
    class Config:
        json_schema_extra = {
            "example": {
                "symbol": "ZC",
                "name": "Corn",
                "available_months": ["March", "May", "July", "September", "December"]
            }
        }